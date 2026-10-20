/**
 * @file NormalStatus.h
 * @brief [T-10] Class View — Tampilan OLED status Normal / Aman
 *
 * Proyek  : PBLIF3-MC09 — Visualisasi Status Deteksi Jamming Spektrum Maritim
 * Board   : ESP32 DevKit V1
 * Penanggung Jawab : Galeh Wibisono (NIM: 3312511092)
 *
 * Tanggung jawab:
 *   - Menampilkan tampilan status "Aman" / Normal pada layar OLED
 *   - Mengaktifkan LED Hijau secara solid (terus-menerus menyala)
 *   - Mematikan LED Kuning dan LED Merah
 */

#ifndef NORMAL_STATUS_H
#define NORMAL_STATUS_H

#include <Adafruit_SSD1306.h>
#include "LedController.h"

class NormalStatus {
public:
    /**
     * @brief Constructor — menerima referensi display OLED dan controller LED.
     * @param display  Referensi objek Adafruit_SSD1306 (diinisialisasi di main.cpp)
     * @param leds     Referensi objek LedController (diinisialisasi di main.cpp)
     */
    NormalStatus(Adafruit_SSD1306 &display, LedController &leds);

    /**
     * @brief Render tampilan status Normal / Aman pada OLED dan nyalakan LED Hijau.
     *
     * Dipanggil setiap siklus loop() saat state aktif = STATE_NORMAL.
     */
    void show();

private:
    Adafruit_SSD1306 &_display;  // Referensi OLED (shared, bukan milik class ini)
    LedController    &_leds;     // Referensi controller LED (shared)
};

#endif // NORMAL_STATUS_H
