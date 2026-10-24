/**
 * @file PeriodicJammingStatus.h
 * @brief [T-12] Class View — Tampilan OLED status Periodic Jamming
 *
 * Proyek  : PBLIF3-MC09 — Visualisasi Status Deteksi Jamming Spektrum Maritim
 * Board   : ESP32 DevKit V1
 * Penanggung Jawab : Muhamad Abid Al Mubarok (NIM: 3312511099)
 *
 * Tanggung jawab:
 *   - Menampilkan peringatan waspada Periodic Jamming pada layar OLED
 *   - Mengaktifkan LED Kuning berkedip ritmis non-blocking (500 ms)
 *   - Mematikan LED Hijau dan LED Merah
 */

#ifndef PERIODIC_JAMMING_STATUS_H
#define PERIODIC_JAMMING_STATUS_H

#include <Adafruit_SSD1306.h>
#include "LedController.h"

class PeriodicJammingStatus {
public:
    /**
     * @brief Constructor — menerima referensi display OLED dan controller LED.
     * @param display  Referensi objek Adafruit_SSD1306 (diinisialisasi di main.cpp)
     * @param leds     Referensi objek LedController (diinisialisasi di main.cpp)
     */
    PeriodicJammingStatus(Adafruit_SSD1306 &display, LedController &leds);

    /**
     * @brief Render tampilan peringatan Periodic Jamming pada OLED dan aktifkan kedip LED Kuning.
     *
     * Dipanggil setiap siklus loop() saat state aktif = STATE_PERIODIC_JAMMING.
     */
    void show();

private:
    Adafruit_SSD1306 &_display;  // Referensi OLED (shared, bukan milik class ini)
    LedController    &_leds;     // Referensi controller LED (shared)
};

#endif // PERIODIC_JAMMING_STATUS_H
