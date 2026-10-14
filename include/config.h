/**
 * @file config.h
 * @brief Single Source of Truth — Konfigurasi Hardware & Parameter Sistem
 *
 * Proyek  : PBLIF3-MC09 — Visualisasi Status Deteksi Jamming Spektrum Maritim
 * Board   : ESP32 DevKit V1 (esp32doit-devkit-v1)
 *
 * ATURAN  : Semua nomor pin GPIO, baudrate, alamat I2C, durasi waktu,
 *           parameter watchdog, dan definisi state WAJIB didefinisikan
 *           di file ini. Dilarang hardcoding di file .cpp manapun.
 */

#ifndef CONFIG_H
#define CONFIG_H

// ═══════════════════════════════════════════════════════════════════
//  PIN GPIO — LED INDIKATOR
// ═══════════════════════════════════════════════════════════════════
#define LED_GREEN_PIN   25   // LED Hijau  — Status Normal / Aman
#define LED_YELLOW_PIN  26   // LED Kuning — Status Periodic Jamming (berkedip)
#define LED_RED_PIN     27   // LED Merah  — Status Constant Jamming (solid)

// ═══════════════════════════════════════════════════════════════════
//  PIN GPIO — OLED SSD1306 (I2C)
// ═══════════════════════════════════════════════════════════════════
#define OLED_SDA_PIN    21   // I2C Data
#define OLED_SCL_PIN    22   // I2C Clock
#define OLED_ADDRESS    0x3C // Alamat I2C OLED SSD1306

// ═══════════════════════════════════════════════════════════════════
//  DIMENSI LAYAR OLED
// ═══════════════════════════════════════════════════════════════════
#define SCREEN_WIDTH    128  // Lebar piksel OLED
#define SCREEN_HEIGHT   64   // Tinggi piksel OLED
#define OLED_RESET      -1   // Reset pin (-1 = shared dengan Arduino reset)

// ═══════════════════════════════════════════════════════════════════
//  SERIAL UART
// ═══════════════════════════════════════════════════════════════════
#define SERIAL_BAUD_RATE  115200  // Kecepatan komunikasi serial (bps)

// ═══════════════════════════════════════════════════════════════════
//  TIMING — DURASI & INTERVAL (dalam milidetik, kecuali WDT)
// ═══════════════════════════════════════════════════════════════════
#define SPLASH_DURATION_MS       3000  // Durasi tampil splash screen (ms)
#define LED_SELFTEST_DURATION_MS 1000  // Durasi self-test LED saat startup (ms)
#define PERIODIC_BLINK_INTERVAL  500   // Interval kedip LED Kuning — Periodic Jamming (ms)
#define DEMO_INTERVAL_MS         4000  // Interval pergantian state pada mode demo (ms)
#define SERIAL_TIMEOUT_MS        10000 // Timeout ketiadaan data serial → "Menunggu Sinyal..." (ms)

// ═══════════════════════════════════════════════════════════════════
//  TASK WATCHDOG TIMER (TWDT) — PROTEKSI AUTO-RECOVERY
// ═══════════════════════════════════════════════════════════════════
#define WDT_TIMEOUT_SECONDS  5  // Batas waktu watchdog sebelum panic reset (detik)

// ═══════════════════════════════════════════════════════════════════
//  ENUM STATE SISTEM
// ═══════════════════════════════════════════════════════════════════
/**
 * @brief Definisi seluruh state yang dikelola oleh system controller (main.cpp).
 *
 * State ditentukan berdasarkan perintah serial UART dari host (Raspberry Pi / PC).
 */
enum SystemState {
    STATE_SPLASH,             // Boot splash screen & self-test LED
    STATE_NORMAL,             // Status Aman — LED Hijau ON, OLED "Status: Aman"
    STATE_CONSTANT_JAMMING,   // Bahaya — LED Merah ON, OLED "PERINGATAN: Constant Jamming"
    STATE_PERIODIC_JAMMING,   // Waspada — LED Kuning Berkedip, OLED "PERINGATAN: Periodic Jamming"
    STATE_WAITING             // Menunggu sinyal — Tidak ada data serial dalam batas timeout
};

#endif // CONFIG_H
