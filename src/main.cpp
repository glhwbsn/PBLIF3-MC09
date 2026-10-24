/**
 * @file main.cpp
 * @brief [T-13] System Controller — Orkestrator utama firmware ESP32
 *
 * Proyek  : PBLIF3-MC09 — Visualisasi Status Deteksi Jamming Spektrum Maritim
 * Board   : ESP32 DevKit V1 (esp32doit-devkit-v1)
 *
 * Penanggung Jawab : Galeh Wibisono (NIM: 3312511092)
 *
 * Tanggung jawab modul ini:
 *   - Inisialisasi seluruh hardware (Serial, I2C, OLED, LED)
 *   - Menerima dan memparsing perintah serial UART dari host
 *   - Merutekan state sistem ke class view (Normal, Constant, Standby)
 *   - Mendeteksi timeout serial (>10 detik) → state STANDBY
 *
 * Fitur aktif:
 *   - [T-10] NormalStatus          (Galeh)  ✓
 *   - [T-11] ConstantJammingStatus (Abid)   ✓
 *   - [T-12] PeriodicJammingStatus (Abid)   ✓
 *   - [T-13] Standby (Waiting)     (Galeh)  ✓
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "config.h"
#include "LedController.h"
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
NormalStatus           normalStatus(display, leds);
ConstantJammingStatus  constantStatus(display, leds);
PeriodicJammingStatus  periodicStatus(display, leds);

// ═══════════════════════════════════════════════════════════════════
//  VARIABEL STATE & TIMING
// ═══════════════════════════════════════════════════════════════════

SystemState   currentState     = STATE_WAITING;  ///< State aktif saat ini
unsigned long lastSerialTime   = 0;              ///< Timestamp data serial terakhir diterima

// ═══════════════════════════════════════════════════════════════════
//  PROTOTIPE FUNGSI
// ═══════════════════════════════════════════════════════════════════

void processSerialCommand(String cmd);
void renderWaitingScreen();
void printHelp();

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

    // ── I2C & OLED SSD1306 ───────────────────────────────────────
    Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println(F("[ERROR] OLED SSD1306 gagal inisialisasi!"));
        Serial.println(F("[ERROR] Periksa koneksi I2C (SDA/SCL) dan alamat 0x3C."));
        // Loop selamanya — tanpa OLED, sistem tidak berguna
        while (true) { delay(1000); }
    }
    Serial.println(F("[OK] OLED SSD1306 terdeteksi pada 0x3C"));

    // ── LED Indikator ────────────────────────────────────────────
    leds.begin();
    Serial.println(F("[OK] LED GPIO 25/26/27 siap"));

    // ── State Awal ───────────────────────────────────────────────
    currentState   = STATE_WAITING;
    lastSerialTime = millis();

    Serial.println();
    Serial.println(F("Sistem siap. Ketik HELP untuk daftar perintah."));
    Serial.println(F("──────────────────────────────────────────"));
}

// ═══════════════════════════════════════════════════════════════════
//  LOOP — SIKLUS UTAMA NON-BLOCKING
// ═══════════════════════════════════════════════════════════════════

void loop() {
    // ── Baca & Proses Perintah Serial ────────────────────────────
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.length() > 0) {
            lastSerialTime = millis();
            processSerialCommand(cmd);
        }
    }

    // ── Deteksi Timeout Serial → STANDBY [T-13] ─────────────────
    if (currentState != STATE_WAITING) {
        if (millis() - lastSerialTime > SERIAL_TIMEOUT_MS) {
            currentState = STATE_WAITING;
            Serial.println(F("[TIMEOUT] Tidak ada data serial > 10 detik → STANDBY"));
        }
    }

    // ── Routing State → Class View ───────────────────────────────
    switch (currentState) {
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

    // ── Update Timer Blink LED (non-blocking) ────────────────────
    leds.update();
}

// ═══════════════════════════════════════════════════════════════════
//  PARSER PERINTAH SERIAL UART
// ═══════════════════════════════════════════════════════════════════

/**
 * @brief Proses perintah string dari Serial UART.
 *
 * Perintah diproses secara case-insensitive setelah trim().
 * Hanya memproses status Normal dan Constant Jamming, serta bantuan.
 */
void processSerialCommand(String cmd) {
    // Konversi ke uppercase untuk perbandingan case-insensitive
    cmd.toUpperCase();

    Serial.print(F("[CMD] Diterima: "));
    Serial.println(cmd);

    // ── NORMAL / 1 / STATUS:NORMAL ───────────────────────────────
    if (cmd == "NORMAL" || cmd == "1" || cmd == "STATUS:NORMAL") {
        currentState = STATE_NORMAL;
        Serial.println(F("[STATE] → NORMAL (LED Hijau ON)"));
    }
    // ── CONSTANT / CONSTANT_JAMMING / 2 ──────────────────────────
    else if (cmd == "CONSTANT" || cmd == "CONSTANT_JAMMING" || cmd == "2") {
        currentState = STATE_CONSTANT_JAMMING;
        Serial.println(F("[STATE] → CONSTANT JAMMING (LED Merah ON)"));
    }
    // ── PERIODIC / PERIODIC_JAMMING / 3 ──────────────────────────
    else if (cmd == "PERIODIC" || cmd == "PERIODIC_JAMMING" || cmd == "3") {
        currentState = STATE_PERIODIC_JAMMING;
        Serial.println(F("[STATE] → PERIODIC JAMMING (LED Kuning Kedip)"));
    }
    // ── HELP — Cetak Menu Bantuan ────────────────────────────────
    else if (cmd == "HELP") {
        printHelp();
    }
    // ── Perintah Tidak Dikenal ───────────────────────────────────
    else {
        Serial.print(F("[?] Perintah tidak dikenal: "));
        Serial.println(cmd);
        Serial.println(F("    Ketik HELP untuk daftar perintah."));
    }
}

// ═══════════════════════════════════════════════════════════════════
//  RENDER LAYAR STANDBY / MENUNGGU SINYAL [T-13]
// ═══════════════════════════════════════════════════════════════════

/**
 * @brief Tampilkan layar "Standby / Belum Dipindai" pada OLED.
 *
 * Layout sesuai spesifikasi AGENTS.md §5.C.5:
 *   +------------------------------+
 *   |       SPEKTRUM MARITIM       |  <- Header sistem (Y: 2)
 *   | ---------------------------- |  <- Garis pemisah atas (Y: 12)
 *   |                              |
 *   |           STANDBY            |  <- Status teks besar size 2 (Y: 18..34)
 *   |    Sinyal Belum Dipindai     |  <- Subteks keterangan (Y: 36)
 *   | ---------------------------- |  <- Garis pemisah bawah (Y: 49)
 *   |      Menunggu Data Host      |  <- Panduan tindakan operator (Y: 54)
 *   +------------------------------+
 */
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

// ═══════════════════════════════════════════════════════════════════
//  CETAK MENU BANTUAN KE SERIAL MONITOR
// ═══════════════════════════════════════════════════════════════════

void printHelp() {
    Serial.println();
    Serial.println(F("===================================================="));
    Serial.println(F(" PILIHAN FITUR STATUS:"));
    Serial.println(F("   [1] NORMAL"));
    Serial.println(F("       -> Status Aman (LED Hijau Solid, OLED: Aman)"));
    Serial.println(F("   [2] CONSTANT JAMMING"));
    Serial.println(F("       -> Status Bahaya (LED Merah Solid, OLED: Bahaya)"));
    Serial.println(F("   [3] PERIODIC JAMMING"));
    Serial.println(F("       -> Status Waspada (LED Kuning Kedip, OLED: Waspada)"));
    Serial.println(F("   * Tidak ada data serial > 10s otomatis kembali ke STANDBY"));
    Serial.println(F("===================================================="));
    Serial.println();
}
