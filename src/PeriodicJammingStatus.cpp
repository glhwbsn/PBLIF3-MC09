/**
 * @file PeriodicJammingStatus.cpp
 * @brief [T-12] Implementasi Class View — Visualisasi Status Periodic Jamming
 *
 * Proyek  : PBLIF3-MC09 — Visualisasi Status Deteksi Jamming Spektrum Maritim
 * Board   : ESP32 DevKit V1
 *
 * Penanggung Jawab : Muhamad Abid Al Mubarok (NIM: 3312511099)
 *
 * Tanggung jawab modul ini:
 *   - Merender tampilan peringatan waspada "Periodic Jamming" pada layar OLED
 *   - Mengaktifkan LED Kuning dalam mode kedip ritmis non-blocking (500 ms)
 *   - Memastikan LED Hijau dan LED Merah dalam keadaan padam
 *
 * Layout OLED 128x64 Piksel (Hierarki 4 Zona Vertikal):
 *   +------------------------------+
 *   |      PERINGATAN WASPADA      |  <- Inverted header bar putih (Y: 0..11)
 *   |                              |
 *   |           PERIODIK           |  <- Status teks besar size 2 (Y: 18..34)
 *   |     Interferensi Berkala     |  <- Subteks keterangan (Y: 36)
 *   | ---------------------------- |  <- Garis pemisah bawah (Y: 49)
 *   |     Paket AIS Terganggu      |  <- Dampak terhadap navigasi (Y: 54)
 *   +------------------------------+
 *
 * Catatan arsitektur:
 *   - Objek Adafruit_SSD1306 di-share via referensi dari main.cpp (tidak dimiliki class ini)
 *   - Kontrol LED didelegasikan sepenuhnya ke LedController (Separation of Concerns)
 *   - Tidak ada hardcoding pin GPIO atau interval waktu (semua dari config.h)
 *   - Method show() bersifat non-blocking dan aman dipanggil setiap siklus loop()
 */

#include "PeriodicJammingStatus.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  CONSTRUCTOR
// ═══════════════════════════════════════════════════════════════════

/**
 * @brief Inisialisasi referensi display OLED dan controller LED.
 *
 * Kedua objek ini dikelola dan diinisialisasi di main.cpp,
 * class ini hanya menyimpan referensi untuk digunakan saat rendering.
 */
PeriodicJammingStatus::PeriodicJammingStatus(Adafruit_SSD1306 &display, LedController &leds)
    : _display(display), _leds(leds) {}

// ═══════════════════════════════════════════════════════════════════
//  METHOD UTAMA — RENDER TAMPILAN PERIODIC JAMMING
// ═══════════════════════════════════════════════════════════════════

/**
 * @brief Render tampilan peringatan waspada Periodic Jamming.
 *
 * Dipanggil setiap siklus loop() ketika state aktif = STATE_PERIODIC_JAMMING.
 * Method ini:
 *   1. Mengatur kondisi LED (Kuning berkedip non-blocking 500 ms, Hijau & Merah OFF)
 *   2. Menggambar seluruh elemen grafis peringatan waspada pada buffer OLED
 *   3. Mengirim buffer ke layar fisik (display.display() sekali saja)
 */
void PeriodicJammingStatus::show() {
    // ── 1. Kontrol Hardware LED ──────────────────────────────────
    // Delegasi ke LedController: LED Kuning berkedip ritmis, Hijau & Merah OFF
    _leds.setPeriodicJamming();

    // ── 2. Render Grafis OLED (128x64 piksel) ────────────────────
    _display.clearDisplay();

    // ── A. Header Bar Terbalik (Zona 1 — Y: 0..11) ───────────────
    // Kotak putih penuh sebagai latar header (Y: 0 s/d 11)
    _display.fillRect(0, 0, SCREEN_WIDTH, 12, SSD1306_WHITE);
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  // Teks hitam di atas putih
    // "PERINGATAN WASPADA" centered (18 char × 6px = 108px → X = 10)
    _display.setCursor(10, 2);
    _display.print(F("PERINGATAN WASPADA"));

    // ── B. Status Utama (Zona 2 — Y: 18..34) ────────────────────
    // Teks "PERIODIK" dengan font besar size 2, centered
    // (8 char × 12px = 96px → X = 16)
    _display.setTextColor(SSD1306_WHITE);  // Kembalikan ke teks putih
    _display.setTextSize(2);
    _display.setCursor(16, 18);
    _display.print(F("PERIODIK"));

    // ── C. Subteks Keterangan (Zona 3 — Y: 36) ──────────────────
    // "Interferensi Berkala" centered (20 char × 6px = 120px → X = 4)
    _display.setTextSize(1);
    _display.setCursor(4, 36);
    _display.print(F("Interferensi Berkala"));

    // ── Garis Pemisah Bawah (Y: 49) ─────────────────────────────
    _display.drawFastHLine(0, 49, SCREEN_WIDTH, SSD1306_WHITE);

    // ── D. Footer Info Gangguan (Zona 4 — Y: 54) ────────────────
    // "Paket AIS Terganggu" centered (19 char × 6px = 114px → X = 7)
    _display.setCursor(7, 54);
    _display.print(F("Paket AIS Terganggu"));

    // ── 3. Kirim Buffer ke Layar Fisik OLED ──────────────────────
    _display.display();
}
