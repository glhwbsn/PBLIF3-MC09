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
 * Layout OLED 128x64 Piksel:
 *   +------------------------------+
 *   | ! PERINGATAN BAHAYA !        |  <- Inverted bar (bg putih, teks hitam)
 *   |                              |
 *   | CONSTANT JAMMING             |  <- Judul status bahaya
 *   | ============================ |  <- Garis pemisah ganda
 *   | SINYAL TERBLOKIR TOTAL       |  <- Keterangan dampak
 *   | Spektrum: Full Noise         |  <- Info level spektrum
 *   | [██████████████████████████] |  <- Bar grafik noise penuh (100%)
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

    // ── A. Header Bar Terbalik (Inverted) ────────────────────────
    // Kotak putih penuh sebagai latar header (Y: 0 s/d 12)
    _display.fillRect(0, 0, SCREEN_WIDTH, 12, SSD1306_WHITE);
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  // Teks hitam di atas putih
    _display.setCursor(6, 2);
    _display.print(F("! PERINGATAN BAHAYA !"));

    // ── B. Judul Status Bahaya ───────────────────────────────────
    _display.setTextColor(SSD1306_WHITE);  // Kembalikan ke teks putih
    _display.setTextSize(1);
    _display.setCursor(4, 16);
    _display.print(F("CONSTANT JAMMING"));

    // ── C. Garis Pemisah Ganda (Double Line Separator) ───────────
    _display.drawFastHLine(0, 26, SCREEN_WIDTH, SSD1306_WHITE);
    _display.drawFastHLine(0, 28, SCREEN_WIDTH, SSD1306_WHITE);

    // ── D. Keterangan Dampak & Info Spektrum ─────────────────────
    _display.setCursor(0, 32);
    _display.print(F("SINYAL TERBLOKIR TOTAL"));

    _display.setCursor(0, 42);
    _display.print(F("Spektrum: Full Noise"));

    // ── E. Bar Grafik Noise Level Penuh (100%) ───────────────────
    // Bingkai luar (outline rectangle)
    _display.drawRect(0, 53, SCREEN_WIDTH, 9, SSD1306_WHITE);
    // Isi penuh di dalam bingkai — representasi noise level 100%
    _display.fillRect(2, 55, SCREEN_WIDTH - 4, 5, SSD1306_WHITE);

    // ── 3. Kirim Buffer ke Layar Fisik OLED ──────────────────────
    _display.display();
}
