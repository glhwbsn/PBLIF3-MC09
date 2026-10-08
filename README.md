# 🛰️ Perangkat Portable Visualisasi Status Deteksi Jamming Spektrum Maritim Berbasis ESP32

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Project-orange.svg?logo=platformio)](https://platformio.org/)
[![Framework](https://img.shields.io/badge/Framework-Arduino-blue.svg?logo=arduino)](https://www.arduino.cc/)
[![Hardware](https://img.shields.io/badge/Board-ESP32%20DevKit%20V1-red.svg?logo=espressif)](https://www.espressif.com/)
[![Display](https://img.shields.io/badge/OLED-SSD1306%20128x64%20I2C-lightblue.svg)](https://learn.adafruit.com/monochrome-oled-breakouts)
[![Institution](https://img.shields.io/badge/Institusi-Politeknik%20Negeri%20Batam-blue)](https://www.polibatam.ac.id/)
[![Team](https://img.shields.io/badge/PBL-PBLIF3--MC09-green.svg)](#-identitas-proyek--tim)

> **Proyek Berbasis Pembelajaran (Project-Based Learning - PBL)**  
> Mata Kuliah: **IF316 - Mata Kuliah Pilihan 1 IoT** (Terintegrasi dengan **IF314 Proyek Inovasi Agile** & **IF315 Rekayasa Perangkat Lunak Lanjut**)  
> Kelas: **IF Malam C** | Semester: **III**  
> Lokasi Proyek: `D:\3312511092\SEM III\IF316 - Mata Kuliah Pilihan 1 IoT\!Proyek\PBLIF3-MC09`

---

## 📌 Daftar Isi
- [Latar Belakang Proyek](#-latar-belakang-proyek)
- [Fitur Utama Sistem](#-fitur-utama-sistem)
- [Struktur Folder Proyek IoT](#-struktur-folder-proyek-iot)
- [Spesifikasi Perangkat Keras & Wiring](#-spesifikasi-perangkat-keras--wiring)
- [Status Output & Logika Indikator (OLED & LED)](#-status-output--logika-indikator-oled--led)
  - [1. Splash Screen & Self-Test (Pengetesan Awal)](#1-splash-screen--self-test-pengetesan-awal-oled--led)
  - [2. Status Normal (Aman)](#2-status-normal-aman)
  - [3. Status Constant Jamming (Bahaya)](#3-status-constant-jamming-bahaya)
  - [4. Status Periodic Jamming (Waspada)](#4-status-periodic-jamming-waspada)
- [Source Code & File Firmware Lengkap](#-source-code--file-firmware-lengkap)
  - [A. platformio.ini](#a-platformioini)
  - [B. include/config.h](#b-includeconfigh)
  - [C. src/main.cpp](#c-srcmaincpp)
  - [D. tools/simulate_pi.py](#d-toolssimulate_pipy)
- [Protokol Komunikasi Data Serial](#-protokol-komunikasi-data-serial)
- [Panduan Instalasi & Menjalankan](#-panduan-instalasi--menjalankan)
  - [Opsi A: Menggunakan PlatformIO (VS Code)](#opsi-a-menggunakan-platformio-vscode---direkomendasikan)
  - [Opsi B: Menggunakan Arduino IDE](#opsi-b-menggunakan-arduino-ide)
- [Pengujian & Simulasi Serial](#-pengujian--simulasi-serial)
- [Identitas Proyek & Tim](#-identitas-proyek--tim)

---

## 🌊 Latar Belakang Proyek

Perairan Batam berbatasan langsung dengan **Selat Malaka**, salah satu selat pelayaran tersibuk di dunia. Keselamatan navigasi kapal sangat bergantung pada integritas transmisi radio taktis:
1. **VHF Channel 16 (156.8 MHz)**: Frekuensi marabahaya (*distress*), panggilan darurat (*mayday, pan-pan, securite*), dan panduan lalu lintas laut (*Vessel Traffic Service / VTS*).
2. **Automatic Identification System (AIS - 161.975 MHz & 162.025 MHz)**: Komunikasi data digital untuk pelacakan koordinat, arah, kecepatan, dan identitas kapal agar tidak terjadi tabrakan.

Ancaman nyata yang dihadapi adalah serangan **Radio Frequency (RF) Jamming**:
* **Constant Jamming**: Penyerang memancarkan gelombang derau bertenaga besar secara terus-menerus yang menutupi sinyal asli dan melumpuhkan komunikasi radio seketika.
* **Periodic Jamming**: Penyerang memancarkan derau secara berkala (*pulsed jamming*), menyebabkan paket data koordinat AIS hilang timbul dan sistem pelacakan tidak sinkron.

**Solusi Proyek:**  
Perangkat ini dirancang sebagai **unit visualizer portabel mandiri berbasis ESP32** yang mendampingi komputer utama pemroses sinyal (*Raspberry Pi / PC*). Sistem ini memvisualisasikan label klasifikasi jamming secara instan melalui **Layar OLED 0.96" SSD1306** dan **3 LED Indikator multiwarna (Hijau, Kuning, Merah)**, sehingga operator navigasi di anjungan kapal dapat langsung mengetahui adanya ancaman tanpa harus terus-menerus memantau layar monitor komputer laboratorium.

---

## ✨ Fitur Utama Sistem

- 🔄 **Self-Test & Splash Screen Otomatis**: Inisialisasi awal layar OLED dan penyalaan serentak seluruh LED untuk memastikan tidak ada komponen yang putus/rusak.
- 🚦 **Indikator Tri-Color LED Tegas**:
  - 🟢 **Hijau (Solid)**: Spektrum Maritim Normal / Kondisi Aman.
  - 🟡 **Kuning (Berkedip Ritmis)**: Terdeteksi *Periodic Jamming* (Interferensi Berkala).
  - 🔴 **Merah (Solid Bahaya)**: Terdeteksi *Constant Jamming* (Sinyal Terblokir Total).
- 🖥️ **Antarmuka Grafis OLED 128x64 Piksel**: Menampilkan label status, keterangan dampak sinyal, dan status kanal radio maritim dengan kontras tinggi.
- ⚡ **Non-Blocking Execution**: Animasi kedip LED dan pembacaan serial menggunakan pewaktu `millis()` sehingga bebas dari `delay()` yang dapat menghambat penerimaan data UART.
- 🔌 **Komunikasi Serial UART Standar (115200 bps)**: Format string data sederhana yang kompatibel secara *plug-and-play* dengan Raspberry Pi, mini PC, maupun mikrokontroler lain.
- 🎮 **Mode Demo Otomatis**: Fitur simulasi mandiri yang dapat diaktifkan lewat perintah `DEMO` untuk mempresentasikan seluruh respon alat secara bergantian tiap 4 detik tanpa komputer utama.
- 🔋 **Daya Mandiri Portabel (USB 5V)**: Aman dan stabil ditenagai langsung menggunakan *powerbank* DC 5V.

---

## 📂 Struktur Folder Proyek IoT

Pohon direktori proyek pada direktori lokal:  
`D:\3312511092\SEM III\IF316 - Mata Kuliah Pilihan 1 IoT\!Proyek\PBLIF3-MC09`

```text
PBLIF3-MC09/
├── .gitignore                     # Filter file build (.pio, .vscode cache)
├── platformio.ini                 # Konfigurasi PlatformIO (board, baudrate, lib_deps)
├── README.md                      # Dokumentasi komprehensif proyek GitHub
│
├── include/                       # Header file & konfigurasi global
│   ├── README                     # Informasi direktori include PlatformIO
│   └── config.h                   # Definisi pin hardware, baudrate, konstanta timing & enum state
│
├── src/                           # Source code firmware utama
│   └── main.cpp                   # Firmware ESP32 (State machine, OLED render, LED control, Serial parser)
│
├── tools/                         # Skrip utilitas pengujian & simulasi
│   └── simulate_pi.py             # Simulator serial Python (simulasi output Raspberry Pi -> ESP32)
│
├── lib/                           # Pustaka kustom tambahan (opsional)
│   └── README
│
└── test/                          # Unit testing PlatformIO
    └── README
```

---

## 🛠️ Spesifikasi Perangkat Keras & Wiring

### 1. Daftar Komponen
1. **ESP32 DOIT DevKit V1** (30 Pin / 38 Pin)
2. **Modul OLED 0.96 Inch Monochrome** (Driver SSD1306, 128x64 Piksel, Antarmuka I2C)
3. **LED Indikator 5mm**:
   - 1x LED Hijau (Status Normal)
   - 1x LED Kuning / Oranye (Status Periodic Jamming)
   - 1x LED Merah (Status Constant Jamming)
4. **3x Resistor 220Ω - 330Ω** (Resistor pembatas arus untuk masing-masing LED)
5. **Breadboard & Kabel Jumper** (atau PCB Prototipe / PCB Matrix)
6. **Kabel Micro-USB** (Daya & Transmisi Data Serial 5V)

### 2. Tabel Pinout Wiring

| Komponen | Pin Modul | Pin ESP32 DevKit V1 | Keterangan |
| :--- | :--- | :--- | :--- |
| **OLED 0.96" SSD1306** | VCC | **3.3V / VIN** | Suplai tegangan 3.3V DC |
| | GND | **GND** | Ground bersama (*common ground*) |
| | SCL | **GPIO 22 (D22)** | I2C Clock Bus |
| | SDA | **GPIO 21 (D21)** | I2C Data Bus |
| **LED Hijau** | Anoda (+) | **GPIO 25 (D25)** | Dihubungkan seri dengan resistor 220Ω - 330Ω |
| | Katoda (-) | **GND** | Ground |
| **LED Kuning** | Anoda (+) | **GPIO 26 (D26)** | Dihubungkan seri dengan resistor 220Ω - 330Ω |
| | Katoda (-) | **GND** | Ground |
| **LED Merah** | Anoda (+) | **GPIO 27 (D27)** | Dihubungkan seri dengan resistor 220Ω - 330Ω |
| | Katoda (-) | **GND** | Ground |

### 3. Diagram Skematik Sirkuit (ASCII Art)

```text
       +-------------------------------------------------------------+
       |                      ESP32 DevKit V1                        |
       |                                                             |
       |  [3V3] -----------------------------------> VCC  (OLED I2C) |
       |  [GND] -----------------------------------> GND  (OLED I2C) |
       |  [D22] (SCL) -----------------------------> SCL  (OLED I2C) |
       |  [D21] (SDA) -----------------------------> SDA  (OLED I2C) |
       |                                                             |
       |  [D25] -----> [220R - 330R] -----> [Anoda (+) LED Hijau]    |
       |                                    [Katoda (-) LED Hijau] -> GND
       |                                                             |
       |  [D26] -----> [220R - 330R] -----> [Anoda (+) LED Kuning]   |
       |                                    [Katoda (-) LED Kuning] -> GND
       |                                                             |
       |  [D27] -----> [220R - 330R] -----> [Anoda (+) LED Merah]    |
       |                                    [Katoda (-) LED Merah]  -> GND
       +-------------------------------------------------------------+
```

---

## 📊 Status Output & Logika Indikator (OLED & LED)

Sistem memiliki **4 kondisi (state)** terprogram:

| No | State Sistem | Masukan Serial UART | Output Layar OLED | Output LED Hijau | Output LED Kuning | Output LED Merah |
| :---: | :--- | :--- | :--- | :---: | :---: | :---: |
| **1** | **Splash Screen & Self-Test** | *Saat Boot / Reset / Perintah `TEST`* | Identitas Proyek & Uji Mandiri OK | 💡 **ON** (Tes) | 💡 **ON** (Tes) | 💡 **ON** (Tes) |
| **2** | **Status Normal** | `NORMAL` atau `1` | `[ NORMAL ]` - Status: Aman | 🟢 **ON (Solid)** | ⚪ OFF | ⚪ OFF |
| **3** | **Constant Jamming** | `CONSTANT_JAMMING` atau `2` | `! PERINGATAN BAHAYA !` - Sinyal Terblokir | ⚪ OFF | ⚪ OFF | 🔴 **ON (Solid)** |
| **4** | **Periodic Jamming** | `PERIODIC_JAMMING` atau `3` | `PERINGATAN WASPADA` - Interferensi Berkala | ⚪ OFF | 🟡 **BLINKING** *(250ms)* | ⚪ OFF |

---

### 1. Splash Screen & Self-Test (Pengetesan Awal OLED & LED)
* **Tujuan**: Menjalankan pengujian fungsionalitas hardware secara mandiri sebelum menerima aliran data serial.
* **Perilaku LED**:
  1. Seluruh LED (Hijau, Kuning, Merah) menyala serentak selama 800 ms (memastikan tidak ada filamen/jalur LED yang putus).
  2. Seluruh LED mati sejenak (200 ms).
  3. LED menyala berurutan bergantian (*running chase*): Hijau (200 ms) $\rightarrow$ Kuning (200 ms) $\rightarrow$ Merah (200 ms) $\rightarrow$ Mati.
* **Tampilan OLED**:
```text
+------------------------------+
| [ PBLIF3 - MC09 ]            |
|                              |
|      MARITIME SPECTRUM       |
|      JAMMING VISUALIZER      |
| ---------------------------- |
|       SYSTEM SELF-TEST       |
|          STATUS: OK          |
+------------------------------+
```

---

### 2. Status Normal (Aman)
* **Kondisi**: Transmisi radio VHF Ch. 16 dan sinyal pelacak AIS berada dalam kondisi spektrum bersih tanpa derau berbahaya.
* **Perilaku LED**: **LED Hijau ON menyala solid**, LED Kuning & Merah OFF.
* **Tampilan OLED**:
```text
+------------------------------+
| SPECTRUM MONITOR             |
| ---------------------------- |
| +--------------------------+ |
| |        NORMAL            | |
| |      [STATUS: AMAN]      | |
| +--------------------------+ |
| ---------------------------- |
| CH16/AIS : NO NOISE          |
+------------------------------+
```

---

### 3. Status Constant Jamming (Bahaya)
* **Kondisi**: Terdeteksi gelombang derau konstan terus-menerus berdaya tinggi yang memblokir saluran komunikasi VHF dan AIS secara total.
* **Perilaku LED**: **LED Merah ON menyala solid**, LED Hijau & Kuning OFF.
* **Tampilan OLED**:
```text
+------------------------------+
| ! PERINGATAN BAHAYA !        |
|                              |
| CONSTANT JAMMING             |
| ============================ |
| SINYAL TERBLOKIR TOTAL       |
| Spektrum: Full Noise         |
| [██████████████████████████] |
+------------------------------+
```

---

### 4. Status Periodic Jamming (Waspada)
* **Kondisi**: Terdeteksi lonjakan derau periodik (*pulsed interference*) yang mengganggu transmisi paket data maritim secara berkala.
* **Perilaku LED**: **LED Kuning berkedip ritmis (*pulsing blink*)** setiap 250 ms secara *non-blocking*, LED Hijau & Merah OFF.
* **Tampilan OLED**:
```text
+------------------------------+
| PERINGATAN WASPADA           |
| ---------------------------- |
| PERIODIC JAMMING             |
|  *   *   *   *   *   *   *   |
| INTERFERENSI BERKALA         |
| Pola: Pulsed RF Peak         |
| ---------------------------- |
+------------------------------+
```

---

## 💻 Source Code & File Firmware Lengkap

Berikut adalah seluruh file implementasi kodingan yang siap dipakai dan telah teruji:

### A. `platformio.ini`
Path: `D:\3312511092\SEM III\IF316 - Mata Kuliah Pilihan 1 IoT\!Proyek\PBLIF3-MC09\platformio.ini`

```ini
; PlatformIO Project Configuration File
; Proyek PBLIF3-MC09: Rancang Bangun Perangkat Portable Visualisasi Status Deteksi Jamming Spektrum Maritim Berbasis ESP32
; Politeknik Negeri Batam - Semester III (IF316 IoT)

[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
monitor_speed = 115200
upload_speed = 921600

lib_deps =
    adafruit/Adafruit GFX Library @ ^1.11.9
    adafruit/Adafruit SSD1306 @ ^2.5.9
```

---

### B. `include/config.h`
Path: `D:\3312511092\SEM III\IF316 - Mata Kuliah Pilihan 1 IoT\!Proyek\PBLIF3-MC09\include\config.h`

```cpp
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ==========================================
// KONFIGURASI SERIAL KOMUNIKASI
// ==========================================
#define SERIAL_BAUD_RATE 115200

// ==========================================
// KONFIGURASI PIN & LAYAR OLED SSD1306 (I2C)
// ==========================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_I2C_ADDRESS 0x3C

// Pin Default I2C ESP32 DevKit V1
#define PIN_OLED_SDA 21
#define PIN_OLED_SCL 22

// ==========================================
// KONFIGURASI PIN INDIKATOR LED
// ==========================================
#define PIN_LED_GREEN  25  // Indikator Status NORMAL (Sinyal Aman)
#define PIN_LED_YELLOW 26  // Indikator Status PERIODIC JAMMING (Waspada)
#define PIN_LED_RED    27  // Indikator Status CONSTANT JAMMING (Bahaya)

// ==========================================
// DEFINISI STATE SISTEM
// ==========================================
enum SystemState {
  STATE_SPLASH,             // Inisialisasi awal & Self-Test
  STATE_NORMAL,             // Kondisi aman / tanpa interferensi
  STATE_CONSTANT_JAMMING,   // Serangan jamming terus-menerus
  STATE_PERIODIC_JAMMING    // Serangan jamming berkala/pulsed
};

// ==========================================
// TIMING KONSTANTA (MILIDETIK)
// ==========================================
#define SPLASH_DURATION_MS       2500  // Durasi tampilan splash screen
#define PERIODIC_BLINK_INTERVAL   250  // Kecepatan kedip LED kuning (ms)
#define DEMO_INTERVAL_MS         4000  // Interval pergantian state pada mode demo

#endif // CONFIG_H
```

---

### C. `src/main.cpp`
Path: `D:\3312511092\SEM III\IF316 - Mata Kuliah Pilihan 1 IoT\!Proyek\PBLIF3-MC09\src\main.cpp`

```cpp
/**
 * ======================================================================================
 * PROYEK PBLIF3-MC09: RANCANG BANGUN PERANGKAT PORTABLE VISUALISASI
 *                     STATUS DETEKSI JAMMING SPEKTRUM MARITIM BERBASIS ESP32
 * ======================================================================================
 * Mata Kuliah : IF316 - Mata Kuliah Pilihan 1 IoT & IF314 Proyek Inovasi Agile
 * Institusi   : Politeknik Negeri Batam (Polibatam)
 * Penulis     : Tim PBLIF3-MC09
 *               - Muhamad Abid Al Mubarok (3312511099)
 *               - Galeh Wibisono (3312511092)
 *
 * Deskripsi   :
 * Firmware ESP32 untuk menerima label klasifikasi kondisi spektrum maritim
 * (AIS / VHF Ch. 16) secara Serial UART (115200 bps) dan memberikan visualisasi
 * seketika melalui layar OLED 0.96" SSD1306 serta indikator 3 LED warna:
 *   1. Splash Screen & Self-Test : Verifikasi inisialisasi OLED & uji fisik LED
 *   2. Status NORMAL             : LED Hijau ON  | OLED: Status Aman / Bersih
 *   3. Status CONSTANT JAMMING   : LED Merah ON  | OLED: Peringatan Constant Jamming
 *   4. Status PERIODIC JAMMING   : LED Kuning Kedip | OLED: Peringatan Periodic Jamming
 * ======================================================================================
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

// Inisialisasi Objek Layar OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// State & Kontrol Variabel
SystemState currentState = STATE_SPLASH;
SystemState previousState = STATE_SPLASH;
bool isDemoMode = false;

// Pewaktu Non-Blocking (millis)
unsigned long splashStartTime = 0;
unsigned long lastBlinkTime = 0;
bool yellowBlinkState = false;
unsigned long lastDemoSwitchTime = 0;
int demoStep = 0;

// Buffer Penerimaan Data Serial
String serialInputBuffer = "";

// Prototipe Fungsi
void initHardware();
void runLedSelfTest();
void showSplashScreen();
void renderNormalStatus();
void renderConstantJammingStatus();
void renderPeriodicJammingStatus();
void updateDisplay(SystemState state);
void updateLeds(SystemState state);
void setSystemState(SystemState newState);
void processSerialCommand(String cmd);
void handleSerialCommunication();
void handleDemoMode();
void printHelpMenu();

void setup() {
  // Inisialisasi Komunikasi Serial UART
  Serial.begin(SERIAL_BAUD_RATE);
  delay(100);

  Serial.println(F("\n======================================================="));
  Serial.println(F(" PBLIF3-MC09: VISUALIZER DETEKSI JAMMING SPEKTRUM MARITIM"));
  Serial.println(F(" Sistem Firmware ESP32 DevKit V1"));
  Serial.println(F("======================================================="));

  // Inisialisasi Pin Hardware & I2C
  initHardware();

  // Jalankan Self-Test LED Fisik
  runLedSelfTest();

  // Tampilkan Splash Screen Awal
  showSplashScreen();
  splashStartTime = millis();

  // Tampilkan Menu Petunjuk Serial
  printHelpMenu();
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Tangani Transisi Selesai Splash Screen
  if (currentState == STATE_SPLASH) {
    if (currentMillis - splashStartTime >= SPLASH_DURATION_MS) {
      Serial.println(F("[SYSTEM] Inisialisasi selesai. Memasuki Status Siaga NORMAL."));
      setSystemState(STATE_NORMAL);
    }
  }

  // 2. Baca dan Proses Perintah Serial Masuk dari Raspberry Pi / PC
  handleSerialCommunication();

  // 3. Tangani Mode Demo Otomatis (jika aktif)
  if (isDemoMode && currentState != STATE_SPLASH) {
    handleDemoMode();
  }

  // 4. Perbarui Logika LED (Termasuk Animasi Non-Blocking untuk Periodic Jamming)
  updateLeds(currentState);
}

/**
 * Inisialisasi Pin GPIO dan Komunikasi Layar OLED
 */
void initHardware() {
  // Atur Mode Pin LED
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);

  // Pastikan seluruh LED mati di awal
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_RED, LOW);

  // Inisialisasi I2C Wire untuk ESP32
  Wire.begin(PIN_OLED_SDA, PIN_OLED_SCL);

  // Inisialisasi Layar OLED SSD1306
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("[ERROR] Gagal mendeteksi layar OLED SSD1306! Periksa wiring I2C."));
    // Berkedip cepat pada LED Merah sebagai tanda error hardware
    while (true) {
      digitalWrite(PIN_LED_RED, HIGH);
      delay(150);
      digitalWrite(PIN_LED_RED, LOW);
      delay(150);
    }
  }

  Serial.println(F("[HARDWARE] Layar OLED SSD1306 terhubung dengan sukses (0x3C)."));
  display.clearDisplay();
  display.display();
}

/**
 * Uji Coba Fisik LED (Self-Test) saat Booting
 * Menyalakan seluruh LED bersamaan, kemudian berurutan untuk verifikasi.
 */
void runLedSelfTest() {
  Serial.println(F("[SELF-TEST] Memulai pengujian fisik LED indikator..."));

  // Tahap 1: Seluruh LED ON serentak (800 ms)
  digitalWrite(PIN_LED_GREEN, HIGH);
  digitalWrite(PIN_LED_YELLOW, HIGH);
  digitalWrite(PIN_LED_RED, HIGH);
  delay(800);

  // Tahap 2: Seluruh LED OFF (200 ms)
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_RED, LOW);
  delay(200);

  // Tahap 3: Running sequence Hijau -> Kuning -> Merah
  digitalWrite(PIN_LED_GREEN, HIGH);
  delay(200);
  digitalWrite(PIN_LED_GREEN, LOW);

  digitalWrite(PIN_LED_YELLOW, HIGH);
  delay(200);
  digitalWrite(PIN_LED_YELLOW, LOW);

  digitalWrite(PIN_LED_RED, HIGH);
  delay(200);
  digitalWrite(PIN_LED_RED, LOW);

  Serial.println(F("[SELF-TEST] Seluruh LED terverifikasi normal."));
}

/**
 * Tampilan Awal (Splash Screen) pada Layar OLED
 */
void showSplashScreen() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Header Box
  display.fillRect(0, 0, SCREEN_WIDTH, 14, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(24, 3);
  display.print(F("PBLIF3 - MC09"));

  // Konten Utama
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(14, 19);
  display.print(F("MARITIME SPECTRUM"));
  display.setCursor(10, 30);
  display.print(F("JAMMING VISUALIZER"));

  display.drawLine(10, 42, 118, 42, SSD1306_WHITE);

  // Footer / Status Boot
  display.setTextSize(1);
  display.setCursor(16, 47);
  display.print(F("SYSTEM SELF-TEST"));
  display.setCursor(35, 56);
  display.print(F("STATUS: OK"));

  display.display();
}

/**
 * Tampilan OLED untuk Status NORMAL (Aman)
 */
void renderNormalStatus() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Header Bar
  display.setTextSize(1);
  display.setCursor(10, 2);
  display.print(F("SPECTRUM MONITOR"));
  display.drawLine(0, 12, SCREEN_WIDTH, 12, SSD1306_WHITE);

  // Frame Box Status
  display.drawRoundRect(4, 16, 120, 32, 4, SSD1306_WHITE);

  // Status Utama
  display.setTextSize(2);
  display.setCursor(28, 22);
  display.print(F("NORMAL"));

  // Detail Status
  display.setTextSize(1);
  display.setCursor(16, 36);
  display.print(F("[STATUS: AMAN]"));

  // Footer Info Frekuensi
  display.drawLine(0, 51, SCREEN_WIDTH, 51, SSD1306_WHITE);
  display.setCursor(4, 55);
  display.print(F("CH16/AIS : NO NOISE"));

  display.display();
}

/**
 * Tampilan OLED untuk Status CONSTANT JAMMING (Bahaya)
 */
void renderConstantJammingStatus() {
  display.clearDisplay();

  // Banner Peringatan Atas (Inverted)
  display.fillRect(0, 0, SCREEN_WIDTH, 14, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 3);
  display.print(F("! PERINGATAN BAHAYA !"));

  // Label Status Utama (Huruf Besar)
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(12, 19);
  display.print(F("CONSTANT JAMMING"));

  // Garis Aksen Ganda
  display.drawLine(10, 30, 118, 30, SSD1306_WHITE);
  display.drawLine(10, 32, 118, 32, SSD1306_WHITE);

  // Keterangan Dampak Sinyal
  display.setCursor(6, 37);
  display.print(F("SINYAL TERBLOKIR TOTAL"));

  display.setCursor(6, 48);
  display.print(F("Spektrum: Full Noise"));

  // Baris status bawah
  display.drawRect(0, 58, SCREEN_WIDTH, 6, SSD1306_WHITE);
  display.fillRect(0, 58, SCREEN_WIDTH, 6, SSD1306_WHITE);

  display.display();
}

/**
 * Tampilan OLED untuk Status PERIODIC JAMMING (Waspada)
 */
void renderPeriodicJammingStatus() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Header Bar
  display.setTextSize(1);
  display.setCursor(12, 2);
  display.print(F("PERINGATAN WASPADA"));
  display.drawLine(0, 12, SCREEN_WIDTH, 12, SSD1306_WHITE);

  // Status Utama
  display.setTextSize(1);
  display.setCursor(12, 18);
  display.print(F("PERIODIC JAMMING"));

  // Visualisasi Pola Jamming Berkala (Dot Pattern)
  for (int x = 12; x < 116; x += 8) {
    display.fillRect(x, 29, 4, 3, SSD1306_WHITE);
  }

  // Keterangan Gangguan Berkala
  display.setCursor(6, 37);
  display.print(F("INTERFERENSI BERKALA"));

  display.setCursor(6, 48);
  display.print(F("Pola: Pulsed RF Peak"));

  display.drawLine(0, 58, SCREEN_WIDTH, 58, SSD1306_WHITE);
  display.setCursor(4, 60);

  display.display();
}

/**
 * Router Pembaharuan Tampilan Layar OLED
 */
void updateDisplay(SystemState state) {
  switch (state) {
    case STATE_SPLASH:
      showSplashScreen();
      break;
    case STATE_NORMAL:
      renderNormalStatus();
      break;
    case STATE_CONSTANT_JAMMING:
      renderConstantJammingStatus();
      break;
    case STATE_PERIODIC_JAMMING:
      renderPeriodicJammingStatus();
      break;
  }
}

/**
 * Kontrol Fisik Lampu LED Berdasarkan State
 */
void updateLeds(SystemState state) {
  unsigned long currentMillis = millis();

  switch (state) {
    case STATE_SPLASH:
      // Diatur terpisah oleh fungsi runLedSelfTest()
      break;

    case STATE_NORMAL:
      // LED Hijau Menyala Solid, yang lain Mati
      digitalWrite(PIN_LED_GREEN, HIGH);
      digitalWrite(PIN_LED_YELLOW, LOW);
      digitalWrite(PIN_LED_RED, LOW);
      break;

    case STATE_CONSTANT_JAMMING:
      // LED Merah Menyala Solid (Tanda Bahaya Aktif), yang lain Mati
      digitalWrite(PIN_LED_GREEN, LOW);
      digitalWrite(PIN_LED_YELLOW, LOW);
      digitalWrite(PIN_LED_RED, HIGH);
      break;

    case STATE_PERIODIC_JAMMING:
      // LED Hijau dan Merah Mati
      digitalWrite(PIN_LED_GREEN, LOW);
      digitalWrite(PIN_LED_RED, LOW);

      // LED Kuning Berkedip Non-Blocking Mengikuti Interval Pulsa
      if (currentMillis - lastBlinkTime >= PERIODIC_BLINK_INTERVAL) {
        lastBlinkTime = currentMillis;
        yellowBlinkState = !yellowBlinkState;
        digitalWrite(PIN_LED_YELLOW, yellowBlinkState ? HIGH : LOW);
      }
      break;
  }
}

/**
 * Ubah State Sistem Secara Aman & Update Layar Seketika
 */
void setSystemState(SystemState newState) {
  currentState = newState;
  updateDisplay(currentState);

  // Reset pewaktu kedip bila beralih ke periodic jamming
  if (newState == STATE_PERIODIC_JAMMING) {
    lastBlinkTime = millis();
    yellowBlinkState = true;
    digitalWrite(PIN_LED_YELLOW, HIGH);
  }
}

/**
 * Parsing dan Eksekusi Perintah String dari Port Serial
 */
void processSerialCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  if (cmd.length() == 0) return;

  Serial.print(F("[COMMAND] Menerima data: \""));
  Serial.print(cmd);
  Serial.println(F("\""));

  if (cmd == "NORMAL" || cmd == "STATUS:NORMAL" || cmd == "1") {
    isDemoMode = false;
    Serial.println(F("[STATE] Beralih ke -> STATUS NORMAL (Hijau ON)"));
    setSystemState(STATE_NORMAL);
  }
  else if (cmd == "CONSTANT" || cmd == "CONSTANT_JAMMING" || cmd == "2") {
    isDemoMode = false;
    Serial.println(F("[STATE] Beralih ke -> CONSTANT JAMMING (Merah ON)"));
    setSystemState(STATE_CONSTANT_JAMMING);
  }
  else if (cmd == "PERIODIC" || cmd == "PERIODIC_JAMMING" || cmd == "3") {
    isDemoMode = false;
    Serial.println(F("[STATE] Beralih ke -> PERIODIC JAMMING (Kuning Kedip)"));
    setSystemState(STATE_PERIODIC_JAMMING);
  }
  else if (cmd == "TEST" || cmd == "SPLASH" || cmd == "0") {
    isDemoMode = false;
    Serial.println(F("[STATE] Menjalankan Ulang Self-Test & Splash Screen..."));
    currentState = STATE_SPLASH;
    runLedSelfTest();
    showSplashScreen();
    splashStartTime = millis();
  }
  else if (cmd == "DEMO") {
    isDemoMode = !isDemoMode;
    lastDemoSwitchTime = millis();
    demoStep = 0;
    Serial.print(F("[MODE] Mode Demo Otomatis: "));
    Serial.println(isDemoMode ? F("AKTIF (Siklus otomatis)") : F("NONAKTIF"));
    if (isDemoMode) {
      setSystemState(STATE_NORMAL);
    }
  }
  else if (cmd == "HELP") {
    printHelpMenu();
  }
  else {
    Serial.print(F("[ERROR] Format data tidak dikenali: "));
    Serial.println(cmd);
    Serial.println(F("Ketik 'HELP' untuk melihat daftar perintah valid."));
  }
}

/**
 * Pembacaan Buffer Serial Masuk secara Asinkron
 */
void handleSerialCommunication() {
  while (Serial.available() > 0) {
    char incomingChar = (char)Serial.read();

    if (incomingChar == '\n' || incomingChar == '\r') {
      if (serialInputBuffer.length() > 0) {
        processSerialCommand(serialInputBuffer);
        serialInputBuffer = "";
      }
    } else {
      // Cegah buffer overflow
      if (serialInputBuffer.length() < 64) {
        serialInputBuffer += incomingChar;
      }
    }
  }
}

/**
 * Otomatisasi Siklus Pergantian State untuk Keperluan Pengujian & Demonstrasi
 */
void handleDemoMode() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastDemoSwitchTime >= DEMO_INTERVAL_MS) {
    lastDemoSwitchTime = currentMillis;
    demoStep = (demoStep + 1) % 3;

    switch (demoStep) {
      case 0:
        Serial.println(F("[DEMO] Siklus 1/3: NORMAL"));
        setSystemState(STATE_NORMAL);
        break;
      case 1:
        Serial.println(F("[DEMO] Siklus 2/3: CONSTANT JAMMING"));
        setSystemState(STATE_CONSTANT_JAMMING);
        break;
      case 2:
        Serial.println(F("[DEMO] Siklus 3/3: PERIODIC JAMMING"));
        setSystemState(STATE_PERIODIC_JAMMING);
        break;
    }
  }
}

/**
 * Cetak Bantuan Perintah Serial ke Terminal
 */
void printHelpMenu() {
  Serial.println(F("\n--- DAFTAR PERINTAH SERIAL UART (115200 bps) ---"));
  Serial.println(F(" 1 atau NORMAL           : Set status NORMAL (LED Hijau ON)"));
  Serial.println(F(" 2 atau CONSTANT_JAMMING : Set status CONSTANT JAMMING (LED Merah ON)"));
  Serial.println(F(" 3 atau PERIODIC_JAMMING : Set status PERIODIC JAMMING (LED Kuning Kedip)"));
  Serial.println(F(" 0 atau TEST / SPLASH    : Jalankan ulang Self-Test LED & Splash Screen"));
  Serial.println(F(" DEMO                    : Toggle siklus pergantian status otomatis"));
  Serial.println(F(" HELP                    : Menampilkan bantuan perintah ini"));
  Serial.println(F("-------------------------------------------------\n"));
}
```

---

### D. `tools/simulate_pi.py`
Path: `D:\3312511092\SEM III\IF316 - Mata Kuliah Pilihan 1 IoT\!Proyek\PBLIF3-MC09\tools\simulate_pi.py`

```python
#!/usr/bin/env python3
"""
======================================================================================
SKRIP SIMULATOR SERIAL PENGIRIM STATUS JAMMING (RASPBERRY PI / PC TO ESP32)
Proyek PBLIF3-MC09: Visualisasi Status Deteksi Jamming Spektrum Maritim
======================================================================================
"""

import sys
import time

try:
    import serial
    import serial.tools.list_ports
except ImportError:
    print("[ERROR] Modul 'pyserial' belum terinstal.")
    print("Silakan jalankan perintah: pip install pyserial")
    sys.exit(1)


def list_available_ports():
    ports = list(serial.tools.list_ports.comports())
    if not ports:
        print("[INFO] Tidak ada port serial yang terdeteksi.")
        return []
    print("\n[INFO] Daftar port serial yang tersedia:")
    for i, port in enumerate(ports):
        print(f"  [{i + 1}] {port.device} - {port.description}")
    return ports


def main():
    print("==================================================================")
    print("  SIMULATOR TRANSMISI DATA SERIAL RASPBERRY PI -> ESP32")
    print("  Proyek PBLIF3-MC09 (Spektrum Maritim Jamming Visualizer)")
    print("==================================================================")

    ports = list_available_ports()
    if not ports:
        print("[!] Hubungkan kabel USB ESP32 ke komputer terlebih dahulu.")
        port_name = input("Atau masukkan nama port manual (misal: COM3 atau /dev/ttyUSB0): ").strip()
    else:
        choice = input(f"\nPilih nomor port [1-{len(ports)}] atau ketik nama port langsung: ").strip()
        if choice.isdigit() and 1 <= int(choice) <= len(ports):
            port_name = ports[int(choice) - 1].device
        else:
            port_name = choice if choice else ports[0].device

    baud_rate = 115200

    try:
        ser = serial.Serial(port_name, baud_rate, timeout=1)
        print(f"\n[+] Berhasil terhubung ke {port_name} pada baudrate {baud_rate}.")
        time.sleep(2)  # Beri jeda ESP32 inisialisasi boot
    except Exception as e:
        print(f"[!] Gagal membuka port {port_name}: {e}")
        sys.exit(1)

    try:
        while True:
            print("\n---------------- MENU SIMULASI PENGIRIMAN ----------------")
            print("  [1] Kirim: NORMAL (Status Aman / LED Hijau)")
            print("  [2] Kirim: CONSTANT_JAMMING (Status Bahaya / LED Merah)")
            print("  [3] Kirim: PERIODIC_JAMMING (Status Waspada / LED Kuning)")
            print("  [4] Kirim: TEST (Self-Test Ulang & Splash Screen)")
            print("  [5] Mode Otomatis (Siklus pengiriman berulang tiap 5 detik)")
            print("  [Q] Keluar dari program")
            print("----------------------------------------------------------")

            pilihan = input("Masukkan pilihan Anda: ").strip().upper()

            if pilihan == "1":
                ser.write(b"NORMAL\n")
                print(">> [TERKIRIM] 'NORMAL'")
            elif pilihan == "2":
                ser.write(b"CONSTANT_JAMMING\n")
                print(">> [TERKIRIM] 'CONSTANT_JAMMING'")
            elif pilihan == "3":
                ser.write(b"PERIODIC_JAMMING\n")
                print(">> [TERKIRIM] 'PERIODIC_JAMMING'")
            elif pilihan == "4":
                ser.write(b"TEST\n")
                print(">> [TERKIRIM] 'TEST' (Self-Test Trigger)")
            elif pilihan == "5":
                print("\n[+] Memulai mode simulasi otomatis berkala (Tekan Ctrl+C untuk stop)...")
                sequence = [
                    ("NORMAL", 5),
                    ("CONSTANT_JAMMING", 5),
                    ("PERIODIC_JAMMING", 5),
                ]
                try:
                    while True:
                        for status, delay_s in sequence:
                            ser.write(f"{status}\n".encode())
                            print(f">> [AUTO] Terkirim: {status} (menunggu {delay_s} detik)")
                            time.sleep(delay_s)
                except KeyboardInterrupt:
                    print("\n[!] Mode otomatis dihentikan.")
            elif pilihan in ("Q", "EXIT"):
                print("[*] Menutup koneksi serial...")
                break
            else:
                cmd = f"{pilihan}\n".encode()
                ser.write(cmd)
                print(f">> [TERKIRIM CUSTOM] '{pilihan}'")

            time.sleep(0.1)

    except KeyboardInterrupt:
        print("\n[*] Program dihentikan pengguna.")
    finally:
        ser.close()
        print("[+] Port serial ditutup. Selesai.")


if __name__ == "__main__":
    main()
```

---

## 📡 Protokol Komunikasi Data Serial

Modul ESP32 menerima data label dari komputer utama (*Raspberry Pi*) melalui antarmuka serial UART (USB):
* **Baud Rate**: `115200 bps`
* **Data Bits**: `8`
* **Parity**: `None`
* **Stop Bits**: `1`
* **Karakter Akhir Baris**: `\n` (*Newline*) atau `\r\n` (*CRLF*)

### Tabel Daftar Perintah:
| Perintah String | Karakter Cepat | Efek pada Sistem Visualizer |
| :--- | :---: | :--- |
| `NORMAL` | `1` | Mengaktifkan Status Normal (LED Hijau ON solid) |
| `CONSTANT_JAMMING` atau `CONSTANT` | `2` | Mengaktifkan Status Constant Jamming (LED Merah ON solid) |
| `PERIODIC_JAMMING` atau `PERIODIC` | `3` | Mengaktifkan Status Periodic Jamming (LED Kuning kedip 250ms) |
| `TEST` atau `SPLASH` | `0` | Menjalankan ulang uji mandiri hardware & splash screen |
| `DEMO` | - | Mengaktifkan/menonaktifkan siklus bergantian status tiap 4 detik |
| `HELP` | - | Menampilkan daftar perintah valid ke Serial Monitor |

---

## 🚀 Panduan Instalasi & Menjalankan

### Opsi A: Menggunakan PlatformIO (VSCode) - *Direkomendasikan*

1. **Buka Proyek**:
   Buka VS Code, pilih **File -> Open Folder**, arahkan ke:
   ```text
   D:\3312511092\SEM III\IF316 - Mata Kuliah Pilihan 1 IoT\!Proyek\PBLIF3-MC09
   ```
2. **Kompilasi (Build)**:
   Klik ikon centang `✓` (**PlatformIO: Build**) pada status bar bawah.
3. **Unggah (Upload)**:
   Hubungkan kabel Micro-USB dari ESP32 ke PC, lalu klik ikon panah `→` (**PlatformIO: Upload**).
4. **Pantau Serial Monitor**:
   Klik ikon colokan kabel (**PlatformIO: Serial Monitor**) pada baudrate `115200`.

---

### Opsi B: Menggunakan Arduino IDE

1. Tambahkan board ESP32 via **Boards Manager** (pilih `DOIT ESP32 DEVKIT V1`).
2. Instal library via **Library Manager**:
   - `Adafruit SSD1306`
   - `Adafruit GFX Library`
3. Salin isi `src/main.cpp` dan `include/config.h` ke dalam satu folder sketch Arduino.
4. Pilih port COM yang terdeteksi, lalu klik **Upload**.

---

## 🧪 Pengujian & Simulasi Serial

1. **Uji Langsung via Serial Monitor**:
   Buka Serial Monitor (115200 bps), lalu kirim:
   - `1` $\rightarrow$ Normal (LED Hijau ON)
   - `2` $\rightarrow$ Constant Jamming (LED Merah ON)
   - `3` $\rightarrow$ Periodic Jamming (LED Kuning Kedip)
   - `DEMO` $\rightarrow$ Siklus otomatis bergantian tiap 4 detik

2. **Uji Otomatis via Skrip Python**:
   Jalankan simulator pengirim status:
   ```bash
   pip install pyserial
   python tools/simulate_pi.py
   ```
   Pilih port COM, lalu gunakan opsi menu interaktif atau opsi `[5]` untuk simulasi otomatis.

---

## 👥 Identitas Proyek & Tim

* **Institusi**: Politeknik Negeri Batam
* **Program Studi**: Sarjana Terapan Rekayasa Perangkat Lunak / Teknik Informatika
* **Mata Kuliah**:
  - IF316 - Mata Kuliah Pilihan 1 IoT
  - IF314 - Proyek Inovasi Agile
  - IF315 - Rekayasa Perangkat Lunak Lanjut
  - IF318 - Statistika
* **Kode Kelompok**: **PBLIF3-MC09** (Kelas IF Malam C)
* **Anggota Pengembang**:
  1. **Muhamad Abid Al Mubarok** - NIM: `3312511099`
  2. **Galeh Wibisono** - NIM: `3312511092`
* **Stakeholder & Pembimbing**:
  - **Pengusul Proyek**: Metta Santiputri, Ph.D.
  - **Klien / Product Owner**: Lettisia Nurdayenti
  - **Manajer Proyek (Manpro)**: Nur Cahyono K., Ph.D.
