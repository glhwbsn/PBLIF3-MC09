/**
 * @file LedController.cpp
 * @brief Implementasi Class Hardware Controller — Pengendali 3x LED Indikator
 *
 * Proyek  : PBLIF3-MC09 — Visualisasi Status Deteksi Jamming Spektrum Maritim
 * Board   : ESP32 DevKit V1 (esp32doit-devkit-v1)
 *
 * ATURAN:
 *   - Semua pin dan interval waktu mengacu pada config.h (Single Source of Truth).
 *   - Tidak boleh ada hardcoding nilai pin atau durasi di file ini.
 *   - Kedipan LED menggunakan millis(), BUKAN delay().
 *   - delay() hanya digunakan pada selfTest() yang dipanggil di setup().
 */

#include "LedController.h"

// ═══════════════════════════════════════════════════════════════════
//  Constructor
// ═══════════════════════════════════════════════════════════════════

LedController::LedController()
    : _isBlinkingYellow(false),
      _yellowState(false),
      _lastBlinkTime(0),
      _isSelfTesting(false),
      _selfTestStartTime(0) {
}

// ═══════════════════════════════════════════════════════════════════
//  Inisialisasi Hardware
// ═══════════════════════════════════════════════════════════════════

void LedController::begin() {
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_YELLOW_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);
    allOff();
}

// ═══════════════════════════════════════════════════════════════════
//  [T-09] Self-Test LED saat Booting
// ═══════════════════════════════════════════════════════════════════

void LedController::selfTest() {
    // Nyalakan ketiga LED serentak
    digitalWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_YELLOW_PIN, HIGH);
    digitalWrite(LED_RED_PIN, HIGH);

    // Tahan selama LED_SELFTEST_DURATION_MS (delay hanya boleh di setup())
    delay(LED_SELFTEST_DURATION_MS);

    // Matikan semua setelah self-test selesai
    allOff();
}

void LedController::startSelfTest() {
    _isBlinkingYellow = false;
    _yellowState = false;
    _isSelfTesting = true;
    _selfTestStartTime = millis();

    // Nyalakan ketiga LED serentak secara non-blocking
    digitalWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_YELLOW_PIN, HIGH);
    digitalWrite(LED_RED_PIN, HIGH);
}

bool LedController::isSelfTesting() const {
    return _isSelfTesting;
}

// ═══════════════════════════════════════════════════════════════════
//  [T-10] Status Normal / Aman — LED Hijau Solid
// ═══════════════════════════════════════════════════════════════════

void LedController::setNormal() {
    _isSelfTesting = false;
    _isBlinkingYellow = false;

    digitalWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);
}

// ═══════════════════════════════════════════════════════════════════
//  [T-11] Status Constant Jamming / Bahaya — LED Merah Solid
// ═══════════════════════════════════════════════════════════════════

void LedController::setConstantJamming() {
    _isSelfTesting = false;
    _isBlinkingYellow = false;

    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_RED_PIN, HIGH);
}

// ═══════════════════════════════════════════════════════════════════
//  [T-12] Status Periodic Jamming / Waspada — LED Kuning Berkedip
// ═══════════════════════════════════════════════════════════════════

void LedController::setPeriodicJamming() {
    _isSelfTesting = false;

    // Jika sudah dalam mode kedip kuning, pertahankan siklus timer blink
    if (_isBlinkingYellow) {
        return;
    }

    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);

    // Aktifkan mode kedip non-blocking
    _isBlinkingYellow = true;
    _yellowState = true;
    _lastBlinkTime = millis();

    // Nyalakan langsung agar respons visual instan
    digitalWrite(LED_YELLOW_PIN, HIGH);
}

// ═══════════════════════════════════════════════════════════════════
//  Matikan Seluruh LED
// ═══════════════════════════════════════════════════════════════════

void LedController::allOff() {
    _isSelfTesting = false;
    _isBlinkingYellow = false;

    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);
}

// ═══════════════════════════════════════════════════════════════════
//  Engine Non-Blocking Self-Test & Blink (Wajib dipanggil di loop())
// ═══════════════════════════════════════════════════════════════════

void LedController::update() {
    unsigned long currentMillis = millis();

    // [T-09] Selesaikan self-test non-blocking setelah LED_SELFTEST_DURATION_MS
    if (_isSelfTesting) {
        if (currentMillis - _selfTestStartTime >= LED_SELFTEST_DURATION_MS) {
            allOff();
        }
        return;
    }

    if (!_isBlinkingYellow) {
        return;  // Tidak ada kedipan aktif, langsung keluar
    }

    if (currentMillis - _lastBlinkTime >= PERIODIC_BLINK_INTERVAL) {
        _lastBlinkTime = currentMillis;
        _yellowState = !_yellowState;
        digitalWrite(LED_YELLOW_PIN, _yellowState ? HIGH : LOW);
    }
}
