/**
 * @file LedController.h
 * @brief Class Hardware Controller — Pengendali 3x LED Indikator
 *
 * Proyek  : PBLIF3-MC09 — Visualisasi Status Deteksi Jamming Spektrum Maritim
 * Board   : ESP32 DevKit V1 (esp32doit-devkit-v1)
 *
 * Tanggung jawab:
 *   - Abstraksi hardware untuk seluruh LED indikator (Hijau, Kuning, Merah)
 *   - Menyediakan prosedur self-test saat booting [T-09]
 *   - Menyediakan pengaturan LED untuk setiap state sistem:
 *       • Normal [T-10]: LED Hijau solid ON
 *       • Constant Jamming [T-11]: LED Merah solid ON
 *       • Periodic Jamming [T-12]: LED Kuning berkedip ritmis non-blocking
 *   - Semua pin dan interval waktu mengacu pada config.h (Single Source of Truth)
 *
 * ATURAN:
 *   - Hanya class ini yang boleh memanggil digitalWrite() untuk pin LED.
 *   - Tidak boleh ada hardcoding pin atau interval di file ini maupun .cpp-nya.
 *   - Kedipan LED menggunakan millis(), BUKAN delay().
 */

#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>
#include "config.h"

class LedController {
public:
    /**
     * @brief Constructor default.
     */
    LedController();

    /**
     * @brief Inisialisasi seluruh pin LED sebagai OUTPUT dan matikan semua LED.
     *
     * Wajib dipanggil sekali di setup() sebelum method lain digunakan.
     */
    void begin();

    /**
     * @brief [T-09] Self-test saat boot: seluruh LED menyala serentak,
     *        kemudian padam setelah LED_SELFTEST_DURATION_MS.
     *
     * Menggunakan delay() — HANYA boleh dipanggil di setup(), bukan di loop().
     */
    void selfTest();

    /**
     * @brief [T-10] Atur LED ke kondisi status Normal / Aman.
     *
     * LED Hijau ON (solid), LED Kuning OFF, LED Merah OFF.
     * Menonaktifkan mode blink jika sebelumnya aktif.
     */
    void setNormal();

    /**
     * @brief [T-11] Atur LED ke kondisi status Constant Jamming / Bahaya.
     *
     * LED Merah ON (solid), LED Hijau OFF, LED Kuning OFF.
     * Menonaktifkan mode blink jika sebelumnya aktif.
     */
    void setConstantJamming();

    /**
     * @brief [T-12] Atur LED ke kondisi status Periodic Jamming / Waspada.
     *
     * LED Hijau OFF, LED Merah OFF, LED Kuning masuk mode kedip ritmis.
     * Interval kedip ditentukan oleh PERIODIC_BLINK_INTERVAL di config.h.
     * Wajib memanggil update() di setiap siklus loop() agar kedipan berjalan.
     */
    void setPeriodicJamming();

    /**
     * @brief Matikan seluruh LED dan nonaktifkan mode blink.
     */
    void allOff();

    /**
     * @brief Rutinitas non-blocking untuk menggerakkan kedipan LED Kuning.
     *
     * Wajib dipanggil di setiap siklus loop(). Hanya melakukan toggle pin
     * LED Kuning jika mode blink aktif dan interval waktu sudah tercapai.
     * Aman dipanggil kapan saja — tidak melakukan apa-apa jika blink tidak aktif.
     */
    void update();

private:
    bool          _isBlinkingYellow;   ///< Flag apakah mode kedip kuning sedang aktif
    bool          _yellowState;        ///< Status fisik pin kuning saat kedip (HIGH/LOW)
    unsigned long _lastBlinkTime;      ///< Timestamp millis() terakhir untuk interval blink
};

#endif // LED_CONTROLLER_H
