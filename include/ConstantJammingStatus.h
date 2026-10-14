/**
 * @file ConstantJammingStatus.h
 * @brief [T-10] Class View — Tampilan OLED status Constant Jamming
 *
 * Proyek  : PBLIF3-MC09 — Visualisasi Status Deteksi Jamming Spektrum Maritim
 * Board   : ESP32 DevKit V1
 *
 * Tanggung jawab:
 *   - Menampilkan peringatan "PERINGATAN: Constant Jamming" pada layar OLED
 *   - Mengaktifkan LED Merah secara solid (terus-menerus menyala)
 *   - Mematikan LED Hijau dan LED Kuning
 */

#ifndef CONSTANT_JAMMING_STATUS_H
#define CONSTANT_JAMMING_STATUS_H

#include <Adafruit_SSD1306.h>
#include "LedController.h"

class ConstantJammingStatus {
public:
    /**
     * @brief Constructor — menerima referensi display OLED dan controller LED.
     * @param display  Referensi objek Adafruit_SSD1306 (diinisialisasi di main.cpp)
     * @param leds     Referensi objek LedController (diinisialisasi di main.cpp)
     */
    ConstantJammingStatus(Adafruit_SSD1306 &display, LedController &leds);

    /**
     * @brief Render tampilan peringatan Constant Jamming pada OLED dan nyalakan LED Merah.
     *
     * Dipanggil setiap siklus loop() saat state aktif = STATE_CONSTANT_JAMMING.
     */
    void show();

private:
    Adafruit_SSD1306 &_display;  // Referensi OLED (shared, bukan milik class ini)
    LedController    &_leds;     // Referensi controller LED (shared)
};

#endif // CONSTANT_JAMMING_STATUS_H
