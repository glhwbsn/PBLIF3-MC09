/**
 * @file ConstantJammingStatus.cpp
 * @brief [T-11] Implementasi Class View — Visualisasi Status Constant Jamming
 *
 * Proyek  : PBLIF3-MC09 — Visualisasi Status Deteksi Jamming Spektrum Maritim
 * Board   : ESP32 DevKit V1
 *
 * Penanggung Jawab : Muhamad Abid Al Mubarok (NIM: 3312511099)
 *
 * Tanggung jawab modul ini:
 *   - Merender tampilan peringatan bahaya "Constant Jamming" pada layar OLED
 *   - Mengaktifkan LED Merah secara solid (terus-menerus menyala)
 *   - Memastikan LED Hijau dan LED Kuning dalam keadaan padam
 *
 * Layout OLED 128x64 Piksel (Hierarki 4 Zona Vertikal):
 *   +------------------------------+
 *   |    ! PERINGATAN BAHAYA !     |  <- Inverted header bar putih (Y: 0..11)
 *   |                              |
 *   |           BAHAYA!            |  <- Status teks besar size 2 (Y: 18..34)
 *   |     Sinyal Blokir Total      |  <- Subteks keterangan (Y: 36)
 *   | ---------------------------- |  <- Garis pemisah bawah (Y: 49)
 *   |      Noise: 100% Putus       |  <- Info padat (17 char, aman no-wrap) (Y: 54)
 *   +------------------------------+
 *
 * Catatan arsitektur:
 *   - Objek Adafruit_SSD1306 di-share via referensi dari main.cpp (tidak dimiliki class ini)
 *   - Kontrol LED didelegasikan sepenuhnya ke LedController (Separation of Concerns)
 *   - Tidak ada hardcoding pin GPIO atau interval waktu (semua dari config.h)
 *   - Method show() bersifat non-blocking dan aman dipanggil setiap siklus loop()
 */

#include "ConstantJammingStatus.h"
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
ConstantJammingStatus::ConstantJammingStatus(Adafruit_SSD1306 &display, LedController &leds)
    : _display(display), _leds(leds) {}

// ═══════════════════════════════════════════════════════════════════
//  METHOD UTAMA — RENDER TAMPILAN CONSTANT JAMMING
// ═══════════════════════════════════════════════════════════════════

/**
 * @brief Render tampilan peringatan bahaya Constant Jamming.
 *
 * Dipanggil setiap siklus loop() ketika state aktif = STATE_CONSTANT_JAMMING.
 * Method ini:
 *   1. Mengatur kondisi LED (Merah ON solid, Hijau & Kuning OFF)
 *   2. Menggambar seluruh elemen grafis peringatan pada buffer OLED
 *   3. Mengirim buffer ke layar fisik (display.display() sekali saja)
 */
void ConstantJammingStatus::show() {
    // ── 1. Kontrol Hardware LED ──────────────────────────────────
    // Delegasi ke LedController: LED Merah ON, Hijau & Kuning OFF
    _leds.setConstantJamming();

    // ── 2. Render Grafis OLED (128x64 piksel) ────────────────────
    _display.clearDisplay();

    // ── A. Header Bar Terbalik (Zona 1 — Y: 0..11) ───────────────
    // Kotak putih penuh sebagai latar header (Y: 0 s/d 11)
    _display.fillRect(0, 0, SCREEN_WIDTH, 12, SSD1306_WHITE);
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  // Teks hitam di atas putih
    // "! PERINGATAN BAHAYA !" centered (21 char × 6px = 126px → X = 1)
    _display.setCursor(1, 2);
    _display.print(F("! PERINGATAN BAHAYA !"));

    // ── B. Status Utama (Zona 2 — Y: 18..34) ────────────────────
    // Teks "BAHAYA!" dengan font besar size 2, centered
    // (7 char × 12px = 84px → X = 22)
    _display.setTextColor(SSD1306_WHITE);  // Kembalikan ke teks putih
    _display.setTextSize(2);
    _display.setCursor(22, 18);
    _display.print(F("BAHAYA!"));

    // ── C. Subteks Keterangan (Zona 3 — Y: 36) ──────────────────
    // "Sinyal Blokir Total" centered (19 char × 6px = 114px → X = 7)
    _display.setTextSize(1);
    _display.setCursor(7, 36);
    _display.print(F("Sinyal Blokir Total"));

    // ── Garis Pemisah Bawah (Y: 49) ─────────────────────────────
    _display.drawFastHLine(0, 49, SCREEN_WIDTH, SSD1306_WHITE);

    // ── D. Footer Info Gangguan (Zona 4 — Y: 54) ────────────────
    // "Noise: 100% Putus" centered (17 char × 6px = 102px → X = 13)
    _display.setCursor(13, 54);
    _display.print(F("Noise: 100% Putus"));

    // ── 3. Kirim Buffer ke Layar Fisik OLED ──────────────────────
    _display.display();
}
