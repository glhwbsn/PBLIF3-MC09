/**
 * @file NormalStatus.cpp
 * @brief [T-10] Implementasi Class View — Visualisasi Status Normal / Aman
 *
 * Proyek  : PBLIF3-MC09 — Visualisasi Status Deteksi Jamming Spektrum Maritim
 * Board   : ESP32 DevKit V1
 *
 * Penanggung Jawab : Galeh Wibisono (NIM: 3312511092)
 *
 * Tanggung jawab modul ini:
 *   - Merender tampilan status "Normal / Aman" pada layar OLED
 *   - Mengaktifkan LED Hijau secara solid (terus-menerus menyala)
 *   - Memastikan LED Kuning dan LED Merah dalam keadaan padam
 *
 * Layout OLED 128x64 Piksel (Hierarki 4 Zona Vertikal):
 *   +------------------------------+
 *   |       SPEKTRUM MARITIM       |  <- Header sistem normal (Y: 2)
 *   | ---------------------------- |  <- Garis pemisah atas (Y: 12)
 *   |                              |
 *   |            NORMAL            |  <- Status teks besar size 2 (Y: 18..34)
 *   |     Status: Sinyal Aman      |  <- Subteks keterangan (Y: 36)
 *   | ---------------------------- |  <- Garis pemisah bawah (Y: 49)
 *   |     VHF Ch.16 & AIS: OK      |  <- Keterangan kanal maritim (Y: 54)
 *   +------------------------------+
 *
 * Catatan arsitektur:
 *   - Objek Adafruit_SSD1306 di-share via referensi dari main.cpp (tidak dimiliki class ini)
 *   - Kontrol LED didelegasikan sepenuhnya ke LedController (Separation of Concerns)
 *   - Tidak ada hardcoding pin GPIO atau interval waktu (semua dari config.h)
 *   - Method show() bersifat non-blocking dan aman dipanggil setiap siklus loop()
 */

#include "NormalStatus.h"
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
NormalStatus::NormalStatus(Adafruit_SSD1306 &display, LedController &leds)
    : _display(display), _leds(leds) {}

// ═══════════════════════════════════════════════════════════════════
//  METHOD UTAMA — RENDER TAMPILAN STATUS NORMAL / AMAN
// ═══════════════════════════════════════════════════════════════════

/**
 * @brief Render tampilan status Normal / Aman pada layar OLED.
 *
 * Dipanggil setiap siklus loop() ketika state aktif = STATE_NORMAL.
 * Method ini:
 *   1. Mengatur kondisi LED (Hijau ON solid, Kuning & Merah OFF)
 *   2. Menggambar seluruh elemen grafis status aman pada buffer OLED
 *   3. Mengirim buffer ke layar fisik (display.display() sekali saja)
 */
void NormalStatus::show() {
    // ── 1. Kontrol Hardware LED ──────────────────────────────────
    // Delegasi ke LedController: LED Hijau ON, Kuning & Merah OFF
    _leds.setNormal();

    // ── 2. Render Grafis OLED (128x64 piksel) ────────────────────
    _display.clearDisplay();

    // ── A. Header Sistem (Zona 1 — Y: 2) ────────────────────────
    // Teks "SPEKTRUM MARITIM" centered (16 char × 6px = 96px → X = 16)
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);
    _display.setCursor(16, 2);
    _display.print(F("SPEKTRUM MARITIM"));

    // ── Garis Pemisah Atas (Y: 12) ──────────────────────────────
    _display.drawFastHLine(0, 12, SCREEN_WIDTH, SSD1306_WHITE);

    // ── B. Status Utama (Zona 2 — Y: 18..34) ────────────────────
    // Teks "NORMAL" dengan font besar size 2, centered
    // (6 char × 12px = 72px → X = 28)
    _display.setTextSize(2);
    _display.setCursor(28, 18);
    _display.print(F("NORMAL"));

    // ── C. Subteks Keterangan (Zona 3 — Y: 36) ──────────────────
    // "Status: Sinyal Aman" centered (19 char × 6px = 114px → X = 7)
    _display.setTextSize(1);
    _display.setCursor(7, 36);
    _display.print(F("Status: Sinyal Aman"));

    // ── Garis Pemisah Bawah (Y: 49) ─────────────────────────────
    _display.drawFastHLine(0, 49, SCREEN_WIDTH, SSD1306_WHITE);

    // ── D. Footer Kanal Maritim (Zona 4 — Y: 54) ────────────────
    // "VHF Ch.16 & AIS: OK" centered (20 char × 6px = 120px → X = 4)
    _display.setCursor(4, 54);
    _display.print(F("VHF Ch.16 & AIS: OK"));

    // ── 3. Kirim Buffer ke Layar Fisik OLED ──────────────────────
    _display.display();
}
