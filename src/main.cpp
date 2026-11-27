#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_system.h>
#include <esp_task_wdt.h>

#include "config.h"
#include "LedController.h"
#include "SplashScreen.h"
#include "NormalStatus.h"
#include "ConstantJammingStatus.h"
#include "PeriodicJammingStatus.h"

// ═══════════════════════════════════════════════════════════════════
//  OBJEK GLOBAL
// ═══════════════════════════════════════════════════════════════════

/// Objek display OLED — diinisialisasi sekali, di-share via referensi
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/// Objek controller LED — abstraksi hardware 3 LED indikator
LedController leds;

/// Objek view — masing-masing menerima referensi display & leds
SplashScreen           splashScreen(display, leds);
NormalStatus           normalStatus(display, leds);
ConstantJammingStatus  constantStatus(display, leds);
PeriodicJammingStatus  periodicStatus(display, leds);

// ═══════════════════════════════════════════════════════════════════
//  VARIABEL STATE, WATCHDOG & SERIAL NON-BLOCKING
// ═══════════════════════════════════════════════════════════════════

SystemState   currentState   = STATE_WAITING;  ///< State aktif saat ini
unsigned long lastSerialTime = 0;              ///< Timestamp data serial valid terakhir

static const size_t SERIAL_BUFFER_MAX = 64;
char   serialBuffer[SERIAL_BUFFER_MAX];
size_t serialBufferIndex = 0;

// ═══════════════════════════════════════════════════════════════════
//  PROTOTIPE FUNGSI
// ═══════════════════════════════════════════════════════════════════

void initWatchdog();
void applyState(SystemState newState, bool forceRender = false);
void handleSerialInput();
void handleSerialTimeout();
void processSerialCommand(String cmd);
void triggerWatchdogFreezeTest();
void renderWaitingScreen();

// ═══════════════════════════════════════════════════════════════════
//  SETUP — INISIALISASI HARDWARE & SUBSISTEM
// ═══════════════════════════════════════════════════════════════════

void setup() {
    // ── Serial UART ──────────────────────────────────────────────
    Serial.begin(SERIAL_BAUD_RATE);
    delay(200);  // Tunggu stabilisasi serial (delay hanya di setup)

    Serial.println();
    Serial.println(F("══════════════════════════════════════════"));
    Serial.println(F("  PBLIF3-MC09 — Jamming Visualizer"));
    Serial.println(F("  Spektrum Maritim — ESP32 DevKit V1"));
    Serial.println(F("══════════════════════════════════════════"));

    if (esp_reset_reason() == ESP_RST_TASK_WDT) {
        Serial.println(F("[RECOVERY] Sistem pulih otomatis dari Task Watchdog Reset!"));
    }

    // ── I2C & OLED SSD1306 ───────────────────────────────────────
    Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println(F("[ERROR] OLED SSD1306 gagal inisialisasi!"));
        Serial.println(F("[ERROR] Periksa koneksi I2C (SDA/SCL) dan alamat 0x3C."));
        // Berhenti sebelum WDT diaktifkan agar pesan galat tetap terbaca tanpa reboot loop
        while (true) { delay(1000); }
    }
    Serial.println(F("[OK] OLED SSD1306 terdeteksi pada 0x3C"));

    // ── LED Indikator ────────────────────────────────────────────
    leds.begin();
    Serial.println(F("[OK] LED GPIO 25/26/27 siap"));

    // ── Task Watchdog Timer (TWDT) [T-13] ────────────────────────
    initWatchdog();

    // ── State Awal: Boot Splash Screen & Self-Test LED [T-09] ────
    lastSerialTime = millis();
    applyState(STATE_SPLASH, true);
    Serial.println(F("[OK] Boot splash screen & self-test LED dimulai"));

    esp_task_wdt_reset();

    Serial.println();
    Serial.println(F("Sistem siap. Perintah: NORMAL, CONSTANT, PERIODIC, FREEZE."));
    Serial.println(F("──────────────────────────────────────────"));
}

// ═══════════════════════════════════════════════════════════════════
//  LOOP — SIKLUS UTAMA NON-BLOCKING
// ═══════════════════════════════════════════════════════════════════

void loop() {
    // ── Feed Task Watchdog Timer di setiap siklus sehat [T-13] ───
    esp_task_wdt_reset();

    // ── Baca & Proses Perintah Serial Non-Blocking ───────────────
    handleSerialInput();

    // ── Deteksi Timeout Serial → STANDBY [T-13] ──────────────────
    handleSerialTimeout();

    // ── Update Timer Self-Test & Blink LED (non-blocking) ────────
    leds.update();

    // ── Update Progres Splash Screen Non-Blocking [T-09] ─────────
    if (currentState == STATE_SPLASH) {
        splashScreen.update();
        if (splashScreen.isFinished()) {
            lastSerialTime = millis();
            applyState(STATE_WAITING);
            Serial.println(F("[SPLASH] Self-test selesai → STANDBY"));
        }
    }
}

// ═══════════════════════════════════════════════════════════════════
//  INISIALISASI WATCHDOG & STATE ROUTING [T-13]
// ═══════════════════════════════════════════════════════════════════

/**
 * @brief Konfigurasi hardware Task Watchdog Timer (TWDT) untuk loopTask.
 */
void initWatchdog() {
    esp_err_t initErr = esp_task_wdt_init(WDT_TIMEOUT_SECONDS, true);
    if (initErr != ESP_OK) {
        Serial.printf("[WARN] TWDT init gagal (err=0x%x)\n", initErr);
        return;
    }

    if (esp_task_wdt_status(NULL) != ESP_OK) {
        esp_err_t addErr = esp_task_wdt_add(NULL);
        if (addErr != ESP_OK) {
            Serial.printf("[WARN] TWDT subscribe loopTask gagal (err=0x%x)\n", addErr);
            return;
        }
    }

    Serial.printf("[OK] Task Watchdog Timer (TWDT) aktif (%d detik)\n", WDT_TIMEOUT_SECONDS);
}

/**
 * @brief Terapkan transisi state dan render OLED hanya saat state berubah.
 */
void applyState(SystemState newState, bool forceRender) {
    if (newState == currentState && !forceRender) {
        return;
    }

    if (currentState == STATE_SPLASH && newState != STATE_SPLASH) {
        splashScreen.stop();
    }

    currentState = newState;

    switch (currentState) {
        case STATE_SPLASH:
            splashScreen.start();
            break;

        case STATE_NORMAL:
            normalStatus.show();
            break;

        case STATE_CONSTANT_JAMMING:
            constantStatus.show();
            break;

        case STATE_PERIODIC_JAMMING:
            periodicStatus.show();
            break;

        case STATE_WAITING:
        default:
            leds.allOff();
            renderWaitingScreen();
            break;
    }
}

// ═══════════════════════════════════════════════════════════════════
//  PENANGANAN SERIAL & TIMEOUT NON-BLOCKING
// ═══════════════════════════════════════════════════════════════════

/**
 * @brief Baca stream serial per byte tanpa blocking hingga newline/CR.
 */
void handleSerialInput() {
    while (Serial.available() > 0) {
        char c = static_cast<char>(Serial.read());

        if (c == '\n' || c == '\r') {
            if (serialBufferIndex == 0) {
                continue;
            }

            serialBuffer[serialBufferIndex] = '\0';
            serialBufferIndex = 0;

            String cmd(serialBuffer);
            cmd.trim();
            if (cmd.length() > 0) {
                processSerialCommand(cmd);
            }
            return;  // Batasi satu perintah per loop agar watchdog & LED tetap tepat waktu
        }

        if (serialBufferIndex < SERIAL_BUFFER_MAX - 1) {
            serialBuffer[serialBufferIndex++] = c;
        }
    }
}

/**
 * @brief Pindahkan sistem ke STANDBY bila tidak ada perintah valid > SERIAL_TIMEOUT_MS.
 */
void handleSerialTimeout() {
    if (currentState == STATE_WAITING || currentState == STATE_SPLASH) {
        return;
    }

    if (millis() - lastSerialTime > SERIAL_TIMEOUT_MS) {
        applyState(STATE_WAITING);
        Serial.printf("[TIMEOUT] Tidak ada data valid > %lu detik → STANDBY\n",
                      static_cast<unsigned long>(SERIAL_TIMEOUT_MS / 1000));
    }
}

// ═══════════════════════════════════════════════════════════════════
//  PARSER PERINTAH SERIAL UART & UJI FAIL-SAFE
// ═══════════════════════════════════════════════════════════════════

/**
 * @brief Proses perintah string dari Serial UART (case-insensitive).
 */
void processSerialCommand(String cmd) {
    cmd.toUpperCase();

    Serial.print(F("[CMD] Diterima: "));
    Serial.println(cmd);

    // ── NORMAL / 1 / STATUS:NORMAL ───────────────────────────────
    if (cmd == "NORMAL" || cmd == "1" || cmd == "STATUS:NORMAL") {
        lastSerialTime = millis();
        applyState(STATE_NORMAL);
        Serial.println(F("[STATE] → NORMAL (LED Hijau ON)"));
    }
    // ── CONSTANT / CONSTANT_JAMMING / 2 ──────────────────────────
    else if (cmd == "CONSTANT" || cmd == "CONSTANT_JAMMING" || cmd == "2") {
        lastSerialTime = millis();
        applyState(STATE_CONSTANT_JAMMING);
        Serial.println(F("[STATE] → CONSTANT JAMMING (LED Merah ON)"));
    }
    // ── PERIODIC / PERIODIC_JAMMING / 3 ──────────────────────────
    else if (cmd == "PERIODIC" || cmd == "PERIODIC_JAMMING" || cmd == "3") {
        lastSerialTime = millis();
        applyState(STATE_PERIODIC_JAMMING);
        Serial.println(F("[STATE] → PERIODIC JAMMING (LED Kuning Kedip)"));
    }
    // ── FREEZE / 9 — Simulasi Hang untuk Uji Auto-Reboot TWDT ────
    else if (cmd == "FREEZE" || cmd == "9") {
        triggerWatchdogFreezeTest();
    }
    // ── Perintah Tidak Dikenal ───────────────────────────────────
    else {
        Serial.print(F("[?] Perintah tidak dikenal: "));
        Serial.println(cmd);
        Serial.println(F("    Perintah valid: NORMAL, CONSTANT, PERIODIC, FREEZE."));
    }
}

/**
 * @brief Mengunci loopTask tanpa feed watchdog untuk membuktikan auto-reboot TWDT.
 */
void triggerWatchdogFreezeTest() {
    Serial.println(F("[TEST] Simulasi FREEZE aktif! Feeding watchdog dihentikan."));
    Serial.printf("[TEST] TWDT akan memicu hardware reboot dalam %d detik...\n", WDT_TIMEOUT_SECONDS);
    Serial.flush();

    while (true) {
        delay(100);  // Menahan loopTask tanpa memanggil esp_task_wdt_reset()
    }
}

// ═══════════════════════════════════════════════════════════════════
//  RENDER LAYAR STANDBY / MENUNGGU SINYAL [T-13]
// ═══════════════════════════════════════════════════════════════════

void renderWaitingScreen() {
    display.clearDisplay();

    // ── A. Header Sistem (Zona 1 — Y: 2) ────────────────────────
    // "SPEKTRUM MARITIM" centered (16 char × 6px = 96px → X = 16)
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(16, 2);
    display.print(F("SPEKTRUM MARITIM"));

    // ── Garis Pemisah Atas (Y: 12) ──────────────────────────────
    display.drawFastHLine(0, 12, SCREEN_WIDTH, SSD1306_WHITE);

    // ── B. Status Utama (Zona 2 — Y: 18..34) ────────────────────
    // "STANDBY" size 2, centered (7 char × 12px = 84px → X = 22)
    display.setTextSize(2);
    display.setCursor(22, 18);
    display.print(F("STANDBY"));

    // ── C. Subteks Keterangan (Zona 3 — Y: 36) ──────────────────
    // "Sinyal Belum Dipindai" (21 char × 6px = 126px → X = 1)
    display.setTextSize(1);
    display.setCursor(1, 36);
    display.print(F("Sinyal Belum Dipindai"));

    // ── Garis Pemisah Bawah (Y: 49) ─────────────────────────────
    display.drawFastHLine(0, 49, SCREEN_WIDTH, SSD1306_WHITE);

    // ── D. Footer Panduan Operator (Zona 4 — Y: 54) ─────────────
    // "Menunggu Data Host" centered (18 char × 6px = 108px → X = 10)
    display.setCursor(10, 54);
    display.print(F("Menunggu Data Host"));

    // ── Kirim Buffer ke Layar Fisik OLED ─────────────────────────
    display.display();
}
