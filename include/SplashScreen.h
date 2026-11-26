/**
 * @file SplashScreen.h
 * @brief [T-09] Class View — Tampilan OLED Boot Splash Screen & Self-Test LED
 *
 * Proyek  : PBLIF3-MC09 — Visualisasi Status Deteksi Jamming Spektrum Maritim
 * Board   : ESP32 DevKit V1
 * Penanggung Jawab : Muhamad Abid Al Mubarok (NIM: 3312511099)
 *
 * Tanggung jawab:
 *   - Menampilkan boot splash screen identitas sistem pada layar OLED SSD1306
 *   - Memicu self-test seluruh LED (Hijau, Kuning, Merah) secara non-blocking
 *   - Melacak durasi splash screen (SPLASH_DURATION_MS) berbasis millis()
 *   - Memperbarui keterangan status self-test OLED saat LED self-test selesai
 */

#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "LedController.h"

class SplashScreen {
public:
    /**
     * @brief Constructor — menerima referensi display OLED dan controller LED.
     * @param display  Referensi objek Adafruit_SSD1306 (diinisialisasi di main.cpp)
     * @param leds     Referensi objek LedController (diinisialisasi di main.cpp)
     */
    SplashScreen(Adafruit_SSD1306 &display, LedController &leds);

    /**
     * @brief Mulai sesi boot splash screen & self-test LED secara non-blocking.
     *
     * Mereset timer millis(), menyalakan ketiga LED melalui LedController::startSelfTest(),
     * dan langsung merender tampilan splash screen ke layar OLED.
     */
    void start();

    /**
     * @brief Render tampilan splash screen ke OLED sesuai status self-test LED saat ini.
     */
    void show();

    /**
     * @brief Rutinitas non-blocking untuk memperbarui footer status saat self-test LED selesai.
     *
     * Dipanggil di setiap siklus loop() selama state aktif = STATE_SPLASH.
     */
    void update();

    /**
     * @brief Cek apakah durasi tampil splash screen (SPLASH_DURATION_MS) sudah terpenuhi.
     * @return true jika sesi splash tidak aktif atau durasi sudah selesai.
     */
    bool isFinished() const;

    /**
     * @brief Hentikan sesi splash screen (misal saat transisi ke state lain).
     */
    void stop();

private:
    Adafruit_SSD1306 &_display;              // Referensi OLED (shared, bukan milik class ini)
    LedController    &_leds;                 // Referensi controller LED (shared)
    unsigned long     _startTime;            // Timestamp millis() saat splash dimulai
    bool              _isActive;             // Flag apakah sesi splash sedang berjalan
    bool              _selfTestDoneRendered; // Flag agar transisi status OK hanya dirender sekali

    /**
     * @brief Helper internal untuk menggambar seluruh elemen grafis splash screen.
     * @param selfTestActive  true saat self-test LED masih aktif, false setelah selesai.
     */
    void render(bool selfTestActive);
};

#endif // SPLASH_SCREEN_H
