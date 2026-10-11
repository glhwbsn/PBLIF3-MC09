# 🛰️ Perangkat Portable Visualisasi Status Deteksi Jamming Spektrum Maritim Berbasis ESP32

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Project-orange.svg?logo=platformio)](https://platformio.org/)
[![Framework](https://img.shields.io/badge/Framework-Arduino-blue.svg?logo=arduino)](https://www.arduino.cc/)
[![Hardware](https://img.shields.io/badge/Board-ESP32%20DevKit%20V1-red.svg?logo=espressif)](https://www.espressif.com/)
[![Display](https://img.shields.io/badge/OLED-SSD1306%20128x64%20I2C-lightblue.svg)](https://learn.adafruit.com/monochrome-oled-breakouts)
[![Safety](https://img.shields.io/badge/Reliability-Task%20Watchdog%20(WDT)-brightgreen.svg)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/wdts.html)
[![Institution](https://img.shields.io/badge/Institusi-Politeknik%20Negeri%20Batam-blue)](https://www.polibatam.ac.id/)
[![Team](https://img.shields.io/badge/PBL-PBLIF3--MC09-green.svg)](#-identitas-proyek--tim)

> **Proyek Berbasis Pembelajaran (Project-Based Learning - PBL)**  
> Mata Kuliah: **IF316 - Mata Kuliah Pilihan 1 IoT** (Terintegrasi dengan **IF314 Proyek Inovasi Agile** & **IF315 Rekayasa Perangkat Lunak Lanjut**)  
> Kelas: **IF Malam C** | Semester: **III**  
> Lokasi Proyek: `D:\3312511092\SEM III\IF316 - Mata Kuliah Pilihan 1 IoT\!Proyek\PBLIF3-MC09`

---

## 📌 Daftar Isi
- [🌊 Latar Belakang Proyek](#-latar-belakang-proyek)
- [✨ Fitur Utama Sistem](#-fitur-utama-sistem)
- [🛡️ Sistem Keamanan & Auto-Recovery (Watchdog Timer - WDT)](#-sistem-keamanan--auto-recovery-watchdog-timer---wdt)
  - [1. Urgensi Watchdog pada Sistem Maritim 24/7](#1-urgensi-watchdog-pada-sistem-maritim-247)
  - [2. Mekanisme Kerja Task Watchdog Timer (TWDT)](#2-mekanisme-kerja-task-watchdog-timer-twdt)
  - [3. Diagram Alur Siklus Watchdog (ASCII Art)](#3-diagram-alur-siklus-watchdog-ascii-art)
  - [4. Implementasi Cuplikan Kode Firmware](#4-implementasi-cuplikan-kode-firmware)
- [📂 Struktur Folder Proyek IoT (Arsitektur OOP Modular)](#-struktur-folder-proyek-iot-arsitektur-oop-modular)
  - [Penjelasan Arsitektur Class OOP](#penjelasan-arsitektur-class-oop)
- [🛠️ Spesifikasi Perangkat Keras & Wiring](#-spesifikasi-perangkat-keras--wiring)
  - [1. Daftar Komponen](#1-daftar-komponen)
  - [2. Tabel Pinout Wiring](#2-tabel-pinout-wiring)
  - [3. Diagram Skematik Sirkuit (ASCII Art)](#3-diagram-skematik-sirkuit-ascii-art)
- [📊 Status Output & Logika Indikator (OLED & LED)](#-status-output--logika-indikator-oled--led)
  - [1. Splash Screen & Self-Test (Pengetesan Awal OLED & LED)](#1-splash-screen--self-test-pengetesan-awal-oled--led)
  - [2. Status Normal (Aman)](#2-status-normal-aman)
  - [3. Status Constant Jamming (Bahaya)](#3-status-constant-jamming-bahaya)
  - [4. Status Periodic Jamming (Waspada)](#4-status-periodic-jamming-waspada)
- [📡 Protokol Komunikasi Data Serial](#-protokol-komunikasi-data-serial)
  - [Tabel Daftar Perintah](#tabel-daftar-perintah)
- [🚀 Panduan Instalasi & Menjalankan](#-panduan-instalasi--menjalankan)
  - [Opsi A: Menggunakan PlatformIO (VS Code) - Direkomendasikan](#opsi-a-menggunakan-platformio-vscode---direkomendasikan)
  - [Opsi B: Menggunakan Arduino IDE](#opsi-b-menggunakan-arduino-ide)
- [🧪 Pengujian & Simulasi Serial](#-pengujian--simulasi-serial)
  - [1. Uji Manual via Serial Monitor](#1-uji-manual-via-serial-monitor)
  - [2. Uji Otomatis via Skrip Python](#2-uji-otomatis-via-skrip-python)
  - [3. Uji Keandalan Watchdog Timer (Simulasi Freeze)](#3-uji-keandalan-watchdog-timer-simulasi-freeze)
- [👥 Identitas Proyek & Tim](#-identitas-proyek--tim)
- [📄 Lisensi](#-lisensi)

---

## 🌊 Latar Belakang Proyek

Perairan Batam berbatasan langsung dengan **Selat Malaka**, salah satu jalur pelayaran internasional tersibuk dan paling strategis di dunia. Keselamatan navigasi pelayaran bertumpu pada integritas frekuensi radio taktis maritim:
1. **VHF Channel 16 (156.8 MHz)**: Saluran marabahaya (*distress*), panggilan darurat (*mayday, pan-pan, securite*), serta layanan pemanduan lalu lintas pelayaran (*Vessel Traffic Service / VTS*).
2. **Automatic Identification System (AIS - 161.975 MHz & 162.025 MHz)**: Komunikasi data digital pelacakan koordinat, arah, kecepatan, dan identitas kapal agar terhindar dari tabrakan di laut lepas.

Ancaman nyata yang dihadapi adalah serangan **Radio Frequency (RF) Jamming**:
* **Constant Jamming**: Pemancaran gelombang derau bertenaga tinggi terus-menerus yang melumpuhkan penerima radio maritim seketika.
* **Periodic Jamming**: Pemancaran derau berkala (*pulsed jamming*), menyebabkan data koordinat kapal pada radar AIS hilang timbul dan sistem pelacakan tidak sinkron.

**Solusi Proyek:**  
Perangkat ini dirancang sebagai **unit visualizer portabel mandiri berbasis ESP32** yang bertindak sebagai antarmuka fisik pendamping komputer pemroses sinyal (*Raspberry Pi / PC*). Sistem ini mengonversi label status klasifikasi spektrum RF menjadi sinyal visual instan melalui **Layar OLED 0.96" SSD1306** dan **3 LED Indikator multiwarna (Hijau, Kuning, Merah)**, sehingga kru di anjungan kapal dapat langsung mengetahui adanya ancaman tanpa harus terus-menerus memantau layar monitor komputer laboratorium.

---

## ✨ Fitur Utama Sistem

- 🔄 **Self-Test & Splash Screen Otomatis**: Inisialisasi awal layar OLED dan penyalaan serentak seluruh LED untuk memastikan tidak ada komponen yang putus/rusak.
- 🚦 **Indikator Tri-Color LED Tegas**:
  - 🟢 **Hijau (Solid)**: Spektrum Maritim Normal / Kondisi Aman.
  - 🟡 **Kuning (Berkedip Ritmis)**: Terdeteksi *Periodic Jamming* (Interferensi Berkala).
  - 🔴 **Merah (Solid Bahaya)**: Terdeteksi *Constant Jamming* (Sinyal Terblokir Total).
- 🖥️ **Antarmuka Grafis OLED 128x64 Piksel**: Menampilkan label status, keterangan dampak sinyal, dan status kanal radio maritim dengan kontras tinggi.
- 🛡️ **Hardware Fail-Safe & Auto-Recovery (Watchdog Timer - WDT)**: Proteksi keandalan perangkat tingkat perangkat keras menggunakan ESP32 Task Watchdog Timer (`esp_task_wdt`). Sistem secara otomatis me-reboot mikrokontroler jika loop program mengalami *deadlock*, *freeze*, atau gangguan bus komunikasi I2C/UART tanpa memerlukan intervensi manusia.
- ⚡ **Non-Blocking Execution**: Animasi kedip LED dan pembacaan serial menggunakan pewaktu `millis()` sehingga bebas dari `delay()` yang dapat menghambat penerimaan data UART maupun memicu timeout watchdog.
- 🔌 **Komunikasi Serial UART Standar (115200 bps)**: Format string data sederhana yang kompatibel secara *plug-and-play* dengan Raspberry Pi, mini PC, maupun mikrokontroler lain.
- 🎮 **Mode Demo Otomatis**: Fitur simulasi mandiri yang dapat diaktifkan lewat perintah `DEMO` untuk mempresentasikan seluruh respon alat secara bergantian tiap 4 detik tanpa komputer utama.
- 🔋 **Daya Mandiri Portabel (USB 5V)**: Aman dan stabil ditenagai langsung menggunakan *powerbank* DC 5V.

---

## 🛡️ Sistem Keamanan & Auto-Recovery (Watchdog Timer - WDT)

### 1. Urgensi Watchdog pada Sistem Maritim 24/7
Dalam skenario operasional anjungan kapal laut, perangkat visualizer maritim dituntut untuk bekerja **tanpa henti (24/7 non-stop)** dan memiliki keandalan tinggi (*high availability*). Risiko yang sering terjadi pada mikrokontroler di lapangan meliputi:
- **Interferensi Gelombang Elektromagnetik (EMI/RFI)** dari radio komunikasi bertenaga besar atau radar kapal yang berpotensi memicu kegagalan bus I2C pada layar OLED (*I2C bus lockup*).
- **Serial Buffer Overflow** akibat lonjakan data serial tak terduga yang mengunci memori mikrokontroler.
- **Deadlock / Infinite Loop** pada eksekusi program.

Tanpa mekanisme proteksi, mikrokontroler yang mengalami *freeze* akan menyebabkan status visualizer membeku (*silent failure*). Hal ini sangat berbahaya karena kru kapal mengira situasi sedang normal padahal sistem navigasi sedang diserang *jamming*. 

Fitur **Watchdog Timer (WDT)** hadir sebagai garis pertahanan terakhir (*fail-safe defense*) untuk memastikan perangkat **selalu pulih secara mandiri (*self-healing*)** dalam hitungan detik.

---

### 2. Mekanisme Kerja Task Watchdog Timer (TWDT)
Pada ESP32, pengamanan diimplementasikan menggunakan **Task Watchdog Timer (TWDT)** bawaan arsitektur ESP-IDF (`esp_task_wdt.h`):
1. **Konfigurasi Timeout**: Watchdog diatur dengan batas waktu toleransi **5 detik** (`WDT_TIMEOUT_SECONDS = 5`).
2. **Pendaftaran Task**: Task utama Arduino (`loopTask`) didaftarkan ke pengawasan TWDT (`esp_task_wdt_add(NULL)`).
3. **Pemberian Makan Watchdog (*Feeding / Petting the Dog*)**: Setiap kali siklus `loop()` berhasil menyelesaikan satu putaran secara normal, fungsi `esp_task_wdt_reset()` dipanggil untuk mereset pewaktu mundur watchdog kembali ke 0.
4. **Pemicu Panic Reset**: Jika sistem mengalami macet (*hang/stuck*) di suatu proses selama lebih dari 5 detik, pewaktu watchdog mencapai batas waktu (*timeout expired*). Chip ESP32 secara instan memicu **Hardware System Reset**, me-reboot CPU, dan menjalankan ulang rutinitas inisialisasi hardware dari awal (*Splash Screen & Self-Test*).

---

### 3. Diagram Alur Siklus Watchdog (ASCII Art)

```text
           +---------------------------------------------------+
           |                 ESP32 Boot / Reset                |
           +---------------------------------------------------+
                                     |
                                     v
           +---------------------------------------------------+
           |            Inisialisasi Hardware & TWDT           |
           |     - esp_task_wdt_init(WDT_TIMEOUT, true)        |
           |     - esp_task_wdt_add(NULL)                      |
           +---------------------------------------------------+
                                     |
                                     v
           +---------------------------------------------------+
           |             Splash Screen & Self-Test             |
           +---------------------------------------------------+
                                     |
                                     v
        +------> +---------------------------------------------------+
        |        |               Eksekusi Rutinitas loop()           |
        |        |  - Parsing data serial dari Raspberry Pi          |
        |        |  - Pembaruan status OLED & animasi LED millis()   |
        |        +---------------------------------------------------+
        |                                  |
   Loop Normal                             v
  (Waktu < 5 Detik) +---------------------------------------------------+
        |           |             "FEED / RESET WATCHDOG"               |
        |           |              esp_task_wdt_reset()                 |
        |           +---------------------------------------------------+
        |                                  |
        +----------------------------------+
                                           |
                                 Jika Terjadi Crash / Hang
                                  (Waktu >= 5 Detik)
                                           |
                                           v
                        [ 💥 WATCHDOG TIMEOUT EXPIRED! ]
                                           |
                                           v
                        [ 🔄 HARDWARE AUTO-REBOOT TRIGGERED ]
                                           |
                                           +----------------------------> (Kembali ke Boot)
```

---

### 4. Implementasi Cuplikan Kode Firmware

Konfigurasi parameter pada `include/config.h`:
```cpp
// ==========================================
// WATCHDOG TIMER CONFIGURATION
// ==========================================
#define WDT_TIMEOUT_SECONDS   5   // Batas waktu timeout sebelum auto-reboot (detik)
```

Integrasi pada `src/main.cpp`:
```cpp
#include <Arduino.h>
#include <esp_task_wdt.h>
#include "config.h"

void setup() {
    Serial.begin(115200);

    // 1. Inisialisasi Task Watchdog Timer (TWDT)
    // Parameter: timeout dalam detik, panic = true (hardware reboot jika timeout)
    esp_task_wdt_init(WDT_TIMEOUT_SECONDS, true);
    esp_task_wdt_add(NULL); // Daftarkan loopTask saat ini ke pengawasan watchdog

    // 2. Inisialisasi hardware (LED, OLED, Self-test)
    // ...
}

void loop() {
    // 3. Reset/Feed Watchdog di setiap siklus loop sehat
    esp_task_wdt_reset();

    // 4. Proses komunikasi serial & render status OLED/LED (non-blocking)
    handleSerialInput();
    updateSystemState();
}
```

---

## 📂 Struktur Folder Proyek IoT (Arsitektur OOP Modular)

Struktur direktori firmware dan modul pendukung pada path lokal:  
`D:\3312511092\SEM III\IF316 - Mata Kuliah Pilihan 1 IoT\!Proyek\PBLIF3-MC09`

```text
PBLIF3-MC09/
├── .gitignore                     # Filter file temporary build (.pio, .vscode cache)
├── platformio.ini                 # Konfigurasi PlatformIO (board, baudrate, lib_deps)
├── README.md                      # Dokumentasi komprehensif proyek GitHub
│
├── include/                       # Header file (.h) - Deklarasi Class & Konfigurasi
│   ├── README                     # Informasi direktori include PlatformIO
│   ├── config.h                   # Definisi pin hardware, baudrate, timing, parameter WDT & enum state
│   ├── LedController.h            # Header Class LedController (pengendali 3 LED)
│   ├── SplashScreen.h             # Header Class SplashScreen (tampilan awal boot)
│   ├── NormalStatus.h             # Header Class NormalStatus ("Status: Aman")
│   ├── ConstantJammingStatus.h    # Header Class ConstantJammingStatus ("PERINGATAN: Constant Jamming")
│   └── PeriodicJammingStatus.h    # Header Class PeriodicJammingStatus ("PERINGATAN: Periodic Jamming")
│
├── src/                           # Source code (.cpp) - Implementasi Logika Class & Main
│   ├── LedController.cpp          # Implementasi fungsi LED (Self-test, Normal, Jamming, Non-blocking blink)
│   ├── SplashScreen.cpp           # Implementasi tampilan grafis boot & identitas sistem
│   ├── NormalStatus.cpp           # Implementasi tampilan OLED status Normal
│   ├── ConstantJammingStatus.cpp  # Implementasi tampilan OLED status Constant Jamming
│   ├── PeriodicJammingStatus.cpp  # Implementasi tampilan OLED status Periodic Jamming
│   └── main.cpp                   # Main program (Orkestrator: inisialisasi WDT, feeding loop, serial parser, state router)
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

### Penjelasan Arsitektur Class OOP:
| Modul / File | Tipe | Tanggung Jawab & Fungsi |
| :--- | :---: | :--- |
| `LedController` (`.h` / `.cpp`) | **Class (Hardware)** | Mengenkapsulasi kendali fisik 3 LED: self-test saat boot, aktivasi LED Hijau solid, LED Merah solid, dan kedip ritmis non-blocking LED Kuning. |
| `SplashScreen` (`.h` / `.cpp`) | **Class (View)** | Mengelola tata letak grafis OLED saat sistem pertama kali menyala: header proyek `PBLIF3-MC09`, teks `SYSTEM SELF-TEST`, dan `STATUS: OK`. |
| `NormalStatus` (`.h` / `.cpp`) | **Class (View)** | Mengelola visualisasi kondisi aman pada OLED: judul monitor, kotak status `NORMAL`, teks `Status: Aman`, dan info frekuensi `CH16/AIS : NO NOISE`. |
| `ConstantJammingStatus` (`.h` / `.cpp`) | **Class (View)** | Mengelola visualisasi bahaya pada OLED: inverted header, teks wajib `PERINGATAN: Constant Jamming`, dan status `SINYAL TERBLOKIR TOTAL`. |
| `PeriodicJammingStatus` (`.h` / `.cpp`) | **Class (View)** | Mengelola visualisasi waspada pada OLED: teks wajib `PERINGATAN: Periodic Jamming`, pola dot interval, dan keterangan `INTERFERENSI BERKALA`. |
| `main.cpp` | **Controller** | Bertindak sebagai orkestrator (*System Controller*): konfigurasi & feeding *Task Watchdog Timer (WDT)*, membaca perintah serial dari Raspberry Pi, memicu transisi state, dan memanggil method class terkait secara efisien. |

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
| **1** | **Splash Screen & Self-Test** | *Saat Boot / Reset / Timeout WDT / Perintah `TEST`* | Identitas Proyek & Uji Mandiri OK | 💡 **ON** (Tes) | 💡 **ON** (Tes) | 💡 **ON** (Tes) |
| **2** | **Status Normal** | `NORMAL` atau `1` | `[ NORMAL ]` - Status: Aman | 🟢 **ON (Solid)** | ⚪ OFF | ⚪ OFF |
| **3** | **Constant Jamming** | `CONSTANT_JAMMING` atau `2` | `! PERINGATAN BAHAYA !` - Sinyal Terblokir | ⚪ OFF | ⚪ OFF | 🔴 **ON (Solid)** |
| **4** | **Periodic Jamming** | `PERIODIC_JAMMING` atau `3` | `PERINGATAN WASPADA` - Interferensi Berkala | ⚪ OFF | 🟡 **BLINKING** *(250ms)* | ⚪ OFF |

---

### 1. Splash Screen & Self-Test (Pengetesan Awal OLED & LED)
* **Tujuan**: Menjalankan pengujian fungsionalitas hardware secara mandiri sebelum menerima aliran data serial (juga dijalankan otomatis pasca auto-reboot oleh Watchdog).
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
| `FREEZE` | `9` | **Simulasi Crash/Hang**: Mengunci loop program untuk memvalidasi interupsi reboot oleh Watchdog Timer |
| `DEMO` | - | Mengaktifkan/menonaktifkan siklus bergantian status tiap 4 detik |
| `HELP` | - | Menampilkan daftar perintah valid ke Serial Monitor |

---

## 🚀 Panduan Instalasi & Menjalankan

### Opsi A: Menggunakan PlatformIO (VS Code) - *Direkomendasikan*

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
3. Salin isi file `src/main.cpp` dan `include/config.h` ke dalam folder sketch Arduino.
4. Pastikan include `<esp_task_wdt.h>` tersedia (sudah built-in di Board Core ESP32).
5. Pilih port COM yang terdeteksi, lalu klik **Upload**.

---

## 🧪 Pengujian & Simulasi Serial

### 1. Uji Manual via Serial Monitor
Buka Serial Monitor (115200 bps), lalu kirim:
- `1` $\rightarrow$ Normal (LED Hijau ON)
- `2` $\rightarrow$ Constant Jamming (LED Merah ON)
- `3` $\rightarrow$ Periodic Jamming (LED Kuning Kedip 250ms)
- `DEMO` $\rightarrow$ Siklus otomatis bergantian tiap 4 detik

### 2. Uji Otomatis via Skrip Python
Jalankan simulator pengirim status:
```bash
pip install pyserial
python tools/simulate_pi.py
```
Pilih port COM, lalu gunakan menu interaktif atau opsi simulasi otomatis berkala.

### 3. Uji Keandalan Watchdog Timer (Simulasi Freeze)
Untuk menguji dan memvalidasi keandalan fitur *Auto-Recovery*:
1. Buka Serial Monitor PlatformIO pada baudrate `115200`.
2. Kirim perintah `FREEZE` atau `9`.
3. Firmware akan dengan sengaja menghentikan *feeding watchdog* (masuk ke infinite blocking loop simulasi).
4. **Hasil**: Tepat setelah **5 detik**, pesan peringatan panik ESP-IDF akan tercetak di serial:
   ```text
   E (XXXXX) task_wdt: Task watchdog got triggered. The following tasks did not reset the watchdog in time:
   E (XXXXX) task_wdt:  - loopTask (CPU 1)
   Rebooting...
   ```
5. Chip ESP32 seketika melakukan reboot otomatis secara mandiri dan kembali menampilkan **Splash Screen & Self-Test** tanpa perlu menekan tombol fisik reset.

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

---

## 📄 Lisensi

Proyek ini dikembangkan untuk keperluan akademik dan penelitian keselamatan navigasi maritim di lingkungan Politeknik Negeri Batam. Hak cipta dilindungi undang-undang.
