/**
 * @file SplashScreen.cpp
 * @brief [T-09] Implementasi Class View — Boot Splash Screen & Self-Test LED
 *
 * Proyek  : PBLIF3-MC09 — Visualisasi Status Deteksi Jamming Spektrum Maritim
 * Board   : ESP32 DevKit V1
 *
 * Penanggung Jawab : Muhamad Abid Al Mubarok (NIM: 3312511099)
 *
 * Tanggung jawab modul ini:
 *   - Merender tampilan boot splash screen identitas sistem pada layar OLED
 *   - Mengaktifkan prosedur self-test seluruh LED melalui LedController secara non-blocking
 *   - Menyediakan tracking durasi SPLASH_DURATION_MS berbasis millis()
 *
 * Layout OLED 128x64 Piksel (Hierarki 4 Zona Vertikal):
 *   +------------------------------+
 *   |      [ PBLIF3 - MC09 ]       |  <- Header bar inverted (Y: 0..11)
 *   |                              |
 *   |           JAMMING            |  <- Judul besar size 2 (Y: 18..34)
 *   |      Visualizer Maritim      |  <- Subjudul size 1 (Y: 36)
 *   | ---------------------------- |  <- Garis pemisah bawah (Y: 49)
 *   |     Self-Test LED: AKTIF     |  <- Info proses self-test (Y: 54)
 *   +------------------------------+
 *
 * Catatan arsitektur:
 *   - Objek Adafruit_SSD1306 di-share via referensi dari main.cpp (tidak dimiliki class ini)
 *   - Kontrol LED didelegasikan sepenuhnya ke LedController (Separation of Concerns)
 *   - Tidak ada hardcoding pin GPIO atau interval waktu (semua dari config.h)
 *   - Bebas delay() sehingga aman untuk Task Watchdog Timer (TWDT) dan pemakaian di loop()
 */

#include "SplashScreen.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  CONSTRUCTOR
// ═══════════════════════════════════════════════════════════════════

/**
 * @brief Inisialisasi referensi display OLED, controller LED, dan state internal.
 */
SplashScreen::SplashScreen(Adafruit_SSD1306 &display, LedController &leds)
    : _display(display),
      _leds(leds),
      _startTime(0),
      _isActive(false),
      _selfTestDoneRendered(false) {
}

// ═══════════════════════════════════════════════════════════════════
//  KONTROL SESI SPLASH SCREEN & SELF-TEST NON-BLOCKING
// ═══════════════════════════════════════════════════════════════════

void SplashScreen::start() {
    _isActive = true;
    _selfTestDoneRendered = false;
    _startTime = millis();

    // Nyalakan ketiga LED serentak untuk self-test non-blocking
    _leds.startSelfTest();

    // Render tampilan splash dengan status self-test aktif
    render(true);
}

void SplashScreen::show() {
    render(_leds.isSelfTesting());
}

void SplashScreen::update() {
    if (!_isActive) {
        return;
    }

    // Setelah durasi LED_SELFTEST_DURATION_MS selesai, perbarui footer sekali saja
    if (!_selfTestDoneRendered && !_leds.isSelfTesting()) {
        _selfTestDoneRendered = true;
        render(false);
    }
}

bool SplashScreen::isFinished() const {
    if (!_isActive) {
        return true;
    }

    return (millis() - _startTime) >= SPLASH_DURATION_MS;
}

void SplashScreen::stop() {
    _isActive = false;
}

// ═══════════════════════════════════════════════════════════════════
//  RENDER GRAFIS OLED (128x64 piksel)
// ═══════════════════════════════════════════════════════════════════

void SplashScreen::render(bool selfTestActive) {
    _display.clearDisplay();

    // ── A. Header Bar Terbalik (Zona 1 — Y: 0..11) ───────────────
    _display.fillRect(0, 0, SCREEN_WIDTH, 12, SSD1306_WHITE);
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    // "[ PBLIF3 - MC09 ]" centered (17 char × 6px = 102px → X = 13)
    _display.setCursor(13, 2);
    _display.print(F("[ PBLIF3 - MC09 ]"));

    // ── B. Judul Utama (Zona 2 — Y: 18..34) ──────────────────────
    // "JAMMING" dengan font besar size 2, centered
    // (7 char × 12px = 84px → X = 22)
    _display.setTextColor(SSD1306_WHITE);
    _display.setTextSize(2);
    _display.setCursor(22, 18);
    _display.print(F("JAMMING"));

    // ── C. Subjudul Sistem (Zona 3 — Y: 36) ──────────────────────
    // "Visualizer Maritim" centered (18 char × 6px = 108px → X = 10)
    _display.setTextSize(1);
    _display.setCursor(10, 36);
    _display.print(F("Visualizer Maritim"));

    // ── Garis Pemisah Bawah (Y: 49) ──────────────────────────────
    _display.drawFastHLine(0, 49, SCREEN_WIDTH, SSD1306_WHITE);

    // ── D. Footer Status Self-Test LED (Zona 4 — Y: 54) ──────────
    if (selfTestActive) {
        // "Self-Test LED: AKTIF" centered (20 char × 6px = 120px → X = 4)
        _display.setCursor(4, 54);
        _display.print(F("Self-Test LED: AKTIF"));
    } else {
        // "Self-Test LED: OK" centered (17 char × 6px = 102px → X = 13)
        _display.setCursor(13, 54);
        _display.print(F("Self-Test LED: OK"));
    }

    // ── Kirim Buffer ke Layar Fisik OLED ─────────────────────────
    _display.display();
}
