# 🛰️ Perangkat Portable Visualisasi Status Deteksi Jamming Spektrum Maritim Berbasis ESP32

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Project-orange.svg?logo=platformio)](https://platformio.org/)
[![Framework](https://img.shields.io/badge/Framework-Arduino-blue.svg?logo=arduino)](https://www.arduino.cc/)
[![Hardware](https://img.shields.io/badge/Board-ESP32%20DevKit%20V1-red.svg?logo=espressif)](https://www.espressif.com/)
[![Display](https://img.shields.io/badge/OLED-SSD1306%20128x64%20I2C-lightblue.svg)](https://learn.adafruit.com/monochrome-oled-breakouts)
[![Safety](https://img.shields.io/badge/Reliability-Task%20Watchdog%20(TWDT)-brightgreen.svg)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/wdts.html)
[![Institution](https://img.shields.io/badge/Institusi-Politeknik%20Negeri%20Batam-blue)](https://www.polibatam.ac.id/)
[![Team](https://img.shields.io/badge/PBL-PBLIF3--MC09-green.svg)](#identitas-proyek-tim--pembagian-tugas)

> **Proyek Berbasis Pembelajaran (Project-Based Learning - PBL)**  
> Mata Kuliah Terintegrasi:  
> - **IF316 - Mata Kuliah Pilihan 1 IoT** (Mata Kuliah Pengampu Utama)  
> - **IF314 - Proyek Inovasi Agile**  
> - **IF315 - Rekayasa Perangkat Lunak Lanjut**  
> - **IF318 - Statistika**  
> Kelas: **IF Malam C** | Semester: **III (Ganjil 2026/2027)**  
> Lokasi Repositori: `D:\3312511092\SEM III\IF316 - Mata Kuliah Pilihan 1 IoT\!Proyek\PBLIF3-MC09`

---

## 📌 Daftar Isi
- [🌊 Latar Belakang Proyek](#latar-belakang-proyek)
- [✨ Fitur Utama Sistem](#fitur-utama-sistem)
- [🛡️ Sistem Keamanan & Auto-Recovery (Watchdog Timer - WDT)](#sistem-keamanan--auto-recovery-watchdog-timer---wdt)
  - [1. Urgensi Watchdog pada Sistem Maritim 24/7](#1-urgensi-watchdog-pada-sistem-maritim-247)
  - [2. Mekanisme Kerja Task Watchdog Timer (TWDT)](#2-mekanisme-kerja-task-watchdog-timer-twdt)
  - [3. Diagram Alur Siklus Watchdog (ASCII Art)](#3-diagram-alur-siklus-watchdog-ascii-art)
  - [4. Implementasi Cuplikan Kode Firmware](#4-implementasi-cuplikan-kode-firmware)
- [📂 Struktur Folder Proyek IoT (Arsitektur OOP Modular)](#struktur-folder-proyek-iot-arsitektur-oop-modular)
  - [Penjelasan Arsitektur Class OOP](#penjelasan-arsitektur-class-oop)
- [🛠️ Spesifikasi Perangkat Keras & Wiring](#spesifikasi-perangkat-keras--wiring)
  - [1. Daftar Komponen](#1-daftar-komponen)
  - [2. Tabel Pinout Wiring](#2-tabel-pinout-wiring)
  - [3. Diagram Skematik Sirkuit (ASCII Art)](#3-diagram-skematik-sirkuit-ascii-art)
- [📊 Status Output & Logika Indikator (OLED & LED)](#status-output--logika-indikator-oled--led)
  - [1. Splash Screen & Self-Test (Pengetesan Awal OLED & LED)](#1-splash-screen--self-test-pengetesan-awal-oled--led)
  - [2. Status Normal (Aman)](#2-status-normal-aman)
  - [3. Status Constant Jamming (Bahaya)](#3-status-constant-jamming-bahaya)
  - [4. Status Periodic Jamming (Waspada)](#4-status-periodic-jamming-waspada)
  - [5. Status Menunggu Sinyal (Standby & Timeout Fail-Safe)](#5-status-menunggu-sinyal-standby--timeout-fail-safe)
- [📡 Protokol Komunikasi Data Serial](#protokol-komunikasi-data-serial)
  - [Tabel Daftar Perintah](#tabel-daftar-perintah)
- [🚀 Panduan Instalasi & Menjalankan](#panduan-instalasi--menjalankan)
  - [Opsi A: Menggunakan PlatformIO (VS Code) - Direkomendasikan](#opsi-a-menggunakan-platformio-vs-code---direkomendasikan)
  - [Opsi B: Menggunakan Arduino IDE](#opsi-b-menggunakan-arduino-ide)
- [🧪 Pengujian & Simulasi Serial](#pengujian--simulasi-serial)
  - [1. Uji Manual via Serial Monitor](#1-uji-manual-via-serial-monitor)
  - [2. Uji Otomatis via Skrip Python](#2-uji-otomatis-via-skrip-python)
  - [3. Uji Keandalan Watchdog Timer (Simulasi Freeze)](#3-uji-keandalan-watchdog-timer-simulasi-freeze)
  - [4. Uji Timeout Fail-Safe Ketiadaan Data Serial](#4-uji-timeout-fail-safe-ketiadaan-data-serial)
- [👥 Identitas Proyek, Tim & Pembagian Tugas](#identitas-proyek-tim--pembagian-tugas)
  - [1. Identitas Akademik](#1-identitas-akademik)
  - [2. Anggota Tim & Pembagian Tugas (Task Assignment)](#2-anggota-tim--pembagian-tugas-task-assignment)
  - [3. Stakeholder & Pembimbing](#3-stakeholder--pembimbing)
- [📄 Lisensi](#lisensi)

---

<a id="latar-belakang-proyek"></a>
## 🌊 Latar Belakang Proyek

Perairan Batam dan Kepulauan Riau berbatasan langsung dengan **Selat Malaka** dan **Selat Singapura**, yang merupakan salah satu koridor pelayaran internasional tersibuk dan paling bernilai strategis di dunia. Kelancaran dan keselamatan operasional navigasi armada kapal maritim sangat bergantung pada ketersediaan dan integritas frekuensi radio taktis:

1. **VHF Marine Channel 16 (156.8 MHz)**: Saluran marabahaya (*distress*), panggilan darurat (*Mayday, Pan-Pan, Securite*), koordinasi SAR maritim, serta layanan pemanduan lalu lintas kapal (*Vessel Traffic Service / VTS*).
2. **Automatic Identification System (AIS - 161.975 MHz & 162.025 MHz)**: Komunikasi data digital broadcast otomatis yang mentransmisikan koordinat posisi GPS, heading, kecepatan, MMSI, dan dimensi kapal guna mencegah insiden tabrakan di laut bebas.

Tantangan dan ancaman nyata dalam spektrum frekuensi radio maritim adalah **Radio Frequency (RF) Jamming**:
* **Constant Jamming**: Emisi gelombang pengacau kontinu bertenaga tinggi yang menyumbat seluruh bandwidth penerima, memicu *denial-of-service* total pada radar AIS dan radio VHF seketika.
* **Periodic Jamming**: Emisi sinyal derau pulsa bersiklus (*pulsed interference*), menyebabkan paket data AIS putus-nyambung, data jejak pelayaran hilang timbul, dan koordinat kapal membeku.

**Solusi Proyek:**  
Perangkat ini dirancang sebagai **unit visualizer portabel mandiri berbasis mikrokontroler ESP32**. Unit ini berfungsi sebagai antarmuka fisik pendamping komputer pemroses sinyal cerdas (*Raspberry Pi / Jetson / Mini PC*). Sistem ini mengonversi hasil inferensi klasifikasi spektrum RF menjadi notifikasi visual real-time melalui **Layar OLED 0.96" SSD1306** dan **3 LED Indikator multiwarna (Hijau, Kuning, Merah)**. Dengan perangkat visualizer ini, perwira anjungan dan operator navigasi dapat memantau kondisi keamanan spektrum seketika dari meja kontrol tanpa harus terus terpaku pada monitor komputer analisis laboratorium.

---

<a id="fitur-utama-sistem"></a>
## ✨ Fitur Utama Sistem

- 🔄 **Self-Test & Splash Screen Otomatis**: Inisialisasi awal layar OLED (3000 ms) serta pengujian penyalaan serentak seluruh LED (1000 ms) saat awal booting untuk memastikan integritas layar dan tidak ada LED yang putus.
- 🚦 **Indikator Tri-Color LED Tegas**:
  - 🟢 **Hijau (Solid)**: Spektrum Maritim Normal / Kondisi Aman (`STATE_NORMAL`).
  - 🟡 **Kuning (Berkedip Ritmis 500 ms)**: Terdeteksi *Periodic Jamming* / Interferensi Berkala (`STATE_PERIODIC_JAMMING`).
  - 🔴 **Merah (Solid Bahaya)**: Terdeteksi *Constant Jamming* / Sinyal Terblokir Total (`STATE_CONSTANT_JAMMING`).
- 🖥️ **Antarmuka Grafis OLED 128x64 Piksel**: Menampilkan teks status kontras tinggi, identitas frekuensi maritim (VHF Ch.16 & AIS), serta grafik visual peringatan bahaya.
- 🛡️ **Hardware Fail-Safe & Auto-Recovery (TWDT 5 Detik)**: Proteksi keandalan perangkat tingkat perangkat keras menggunakan **Task Watchdog Timer (TWDT)** bawaan ESP-IDF (`esp_task_wdt.h`). Sistem otomatis melakukan *hardware reboot* jika loop program mengalami *deadlock*, *freeze*, atau gangguan bus komunikasi I2C/UART tanpa memerlukan intervensi manusia.
- ⏱️ **Serial Timeout & Fail-Safe State (10 Detik)**: Sistem secara cerdas mendeteksi ketiadaan aliran data serial dari host komputer selama lebih dari 10 detik (`SERIAL_TIMEOUT_MS`). Sistem secara otomatis berpindah ke `STATE_WAITING` (*"Menunggu Sinyal..."*) guna mencegah informasi basi (*stale data*).
- ⚡ **Non-Blocking Execution (`millis()`)**: Animasi kedip LED, penghitungan timeout serial, dan pembacaan stream serial menggunakan penjadwalan waktu non-blocking `millis()`, sepenuhnya bebas dari fungsi `delay()` yang dapat mengunci eksekusi mikrokontroler.
- 🔌 **Komunikasi Serial UART Standar (115200 bps)**: Format string data sederhana dan case-insensitive yang kompatibel secara *plug-and-play* dengan Raspberry Pi, mini PC, Python script, maupun mikrokontroler lain.
- 🎮 **Mode Demo Otomatis**: Fitur demonstrasi mandiri yang dapat diaktifkan lewat perintah `DEMO` untuk mempresentasikan seluruh respon alat secara bergantian tiap 4 detik tanpa memerlukan komputer host.
- 🔋 **Daya Mandiri Portabel (USB 5V)**: Efisien dan stabil ditenagai langsung menggunakan *powerbank* DC 5V maupun port USB host.

---

<a id="sistem-keamanan-auto-recovery-watchdog-timer-wdt"></a>
<a id="sistem-keamanan--auto-recovery-watchdog-timer---wdt"></a>
## 🛡️ Sistem Keamanan & Auto-Recovery (Watchdog Timer - WDT)

<a id="1-urgensi-watchdog-pada-sistem-maritim-247"></a>
### 1. Urgensi Watchdog pada Sistem Maritim 24/7

Dalam skenario operasional anjungan kapal laut, perangkat visualizer maritim dituntut untuk bekerja **tanpa henti (24/7 non-stop)** dengan keandalan tinggi (*high availability*). Risiko yang kerap terjadi pada perangkat mikrokontroler di laut lepas meliputi:
- **Interferensi Gelombang Elektromagnetik (EMI/RFI)** dari radar navigasi berdaya tinggi atau pemancar radio bertenaga besar yang berpotensi mengunci bus komunikasi I2C pada layar OLED (*I2C bus lockup*).
- **Serial Buffer Overflow / Glitch** akibat lonjakan data serial tak terduga yang mengunci memori mikrokontroler.
- **Deadlock / Infinite Loop** akibat kegagalan eksekusi rutinitas program.

Tanpa mekanisme proteksi, mikrokontroler yang mengalami macet (*freeze*) akan menyebabkan indikator visual membeku pada status terakhir (*silent failure*). Hal ini sangat berbahaya karena kru kapal mengira kondisi spektrum sedang aman padahal navigasi sedang diserang jamming.

Fitur **Watchdog Timer (WDT)** hadir sebagai garis pertahanan terakhir (*fail-safe defense*) untuk menjamin perangkat **selalu pulih secara mandiri (*self-healing*)** dalam hitungan detik.

---

<a id="2-mekanisme-kerja-task-watchdog-timer-twdt"></a>
### 2. Mekanisme Kerja Task Watchdog Timer (TWDT)

Pada ESP32, pengamanan diimplementasikan menggunakan **Task Watchdog Timer (TWDT)** bawaan subsistem ESP-IDF (`esp_task_wdt.h`):
1. **Konfigurasi Timeout**: Watchdog diatur dengan batas waktu toleransi **5 detik** (`WDT_TIMEOUT_SECONDS = 5`).
2. **Pendaftaran Task**: Task utama Arduino (`loopTask`) didaftarkan ke pengawasan TWDT (`esp_task_wdt_add(NULL)`).
3. **Pemberian Makan Watchdog (*Feeding / Petting the Dog*)**: Setiap kali siklus `loop()` berhasil menyelesaikan satu putaran secara normal, fungsi `esp_task_wdt_reset()` dipanggil untuk mereset pewaktu mundur watchdog kembali ke 0.
4. **Pemicu Panic Reset**: Jika sistem mengalami macet (*hang/stuck*) di suatu proses selama lebih dari 5 detik, pewaktu watchdog mencapai batas waktu (*timeout expired*). Chip ESP32 secara instan memicu **Hardware System Reset**, me-reboot CPU, dan menjalankan ulang rutinitas inisialisasi hardware dari awal (*Splash Screen & Self-Test*).

---

<a id="3-diagram-alur-siklus-watchdog-ascii-art"></a>
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

<a id="4-implementasi-cuplikan-kode-firmware"></a>
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
    Serial.begin(SERIAL_BAUD_RATE);

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

<a id="struktur-folder-proyek-iot-arsitektur-oop-modular"></a>
## 📂 Struktur Folder Proyek IoT (Arsitektur OOP Modular)

Struktur direktori firmware dan modul pendukung pada path lokal:  
`D:Ù2511092\SEM III\IF316 - Mata Kuliah Pilihan 1 IoT\!Proyek\PBLIF3-MC09`

```text
PBLIF3-MC09/
├── platformio.ini                 # Konfigurasi PlatformIO (board, framework, baudrate, lib_deps)
├── README.md                      # Dokumentasi komprehensif proyek GitHub
├── AGENTS.md                      # Panduan instruksi, arsitektur, dan SOP untuk coding agent
│
├── include/                       # Header file (.h) - Deklarasi Class & Konfigurasi Sistem
│   ├── README                     # Informasi direktori include PlatformIO
│   ├── config.h                   # Single Source of Truth: Definisi pin, baudrate, timing, WDT & enum state
│   ├── LedController.h            # Header Class Hardware: Pengendali fisik 3 LED (self-test, solid, blink)
│   ├── SplashScreen.h             # Header Class View: Tampilan boot splash screen & identitas sistem
│   ├── NormalStatus.h             # Header Class View: Tampilan OLED status Normal ("Status: Aman")
│   ├── ConstantJammingStatus.h    # Header Class View: Tampilan OLED status Constant Jamming (Bahaya)
│   └── PeriodicJammingStatus.h    # Header Class View: Tampilan OLED status Periodic Jamming (Waspada)
│
├── src/                           # Source code (.cpp) - Implementasi Logika Class & Main
│   ├── LedController.cpp          # Implementasi fungsi kontrol fisik LED & timer blink non-blocking
│   ├── SplashScreen.cpp           # Implementasi tata letak visual splash screen & self-test
│   ├── NormalStatus.cpp           # Implementasi visualisasi kondisi spektrum normal pada OLED
│   ├── ConstantJammingStatus.cpp  # Implementasi visualisasi bahaya constant jamming pada OLED
│   ├── PeriodicJammingStatus.cpp  # Implementasi visualisasi waspada periodic jamming pada OLED
│   └── main.cpp                   # Orkestrator Sistem: Inisialisasi WDT, feeding loop, serial parser & state router
│
├── tools/                         # Skrip utilitas pengujian & simulasi serial UART
│   └── simulate_pi.py             # Simulator serial Python (simulasi pengirim label Raspberry Pi -> ESP32)
│
├── lib/                           # Pustaka kustom tambahan (opsional)
│   └── README
│
└── test/                          # Unit testing PlatformIO
    └── README
```

<a id="penjelasan-arsitektur-class-oop"></a>
### Penjelasan Arsitektur Class OOP

| Modul / File | Tipe Komponen | Tanggung Jawab & Fungsi Utama | Penanggung Jawab |
| :--- | :---: | :--- | :---: |
| `config.h` | **Config (SSOT)** | Mendefinisikan seluruh nomor pin GPIO, baudrate serial (115200), durasi splash (3000ms), durasi self-test (1000ms), interval blink kuning (500ms), timeout serial (10000ms), timeout TWDT (5s), dan enum `SystemState`. Dilarang hardcoding di file .cpp. | Galeh Wibisono |
| `LedController` (`.h` / `.cpp`) | **Class (Hardware)** | Mengenkapsulasi kontrol fisik 3 LED: rutinitas self-test startup, aktivasi LED Hijau solid, LED Merah solid, kedipan ritmis non-blocking LED Kuning (`millis()`), dan pemadaman LED. | Galeh Wibisono |
| `SplashScreen` (`.h` / `.cpp`) | **Class (View)** | Mengelola perancangan grafis OLED saat sistem pertama kali menyala: teks header `PBLIF3-MC09`, judul `MARITIME SPECTRUM JAMMING VISUALIZER`, serta baris status `SYSTEM SELF-TEST OK`. | Galeh Wibisono |
| `NormalStatus` (`.h` / `.cpp`) | **Class (View)** | Mengelola visualisasi kondisi aman pada OLED: banner `SPECTRUM MONITOR`, kotak status `NORMAL`, teks `Status: Aman`, dan info frekuensi `CH16/AIS : NO NOISE`. | Galeh Wibisono |
| `ConstantJammingStatus` (`.h` / `.cpp`) | **Class (View)** | Mengelola visualisasi bahaya pada OLED: inverted header `! PERINGATAN BAHAYA !`, judul `CONSTANT JAMMING`, teks `SINYAL TERBLOKIR TOTAL`, dan bar grafik derau spektrum penuh. | Muhamad Abid Al M. |
| `PeriodicJammingStatus` (`.h` / `.cpp`) | **Class (View)** | Mengelola visualisasi waspada pada OLED: banner `PERINGATAN WASPADA`, judul `PERIODIC JAMMING`, indikator pulsa `* * * * *`, dan keterangan `INTERFERENSI BERKALA`. | Muhamad Abid Al M. |
| `main.cpp` | **Controller** | Orkestrator utama: menginisialisasi Task Watchdog Timer (TWDT), feeding watchdog rutin di setiap putaran `loop()`, mem-parsing perintah serial non-blocking, mendeteksi timeout aliran serial, dan merouting transisi state sistem. | Galeh Wibisono & Integrasi |

---

<a id="spesifikasi-perangkat-keras-wiring"></a>
<a id="spesifikasi-perangkat-keras--wiring"></a>
## 🛠️ Spesifikasi Perangkat Keras & Wiring

<a id="1-daftar-komponen"></a>
### 1. Daftar Komponen

1. **ESP32 DOIT DevKit V1** (30 Pin / 38 Pin) — Mikrokontroler utama berbasis ESP-WROOM-32.
2. **Modul Layar OLED 0.96 Inch Monochrome** — Resolusi 128x64 Piksel, Driver IC SSD1306, Antarmuka I2C.
3. **LED Indikator 5mm**:
   - 1x LED Hijau (Indikator Status Normal / Aman)
   - 1x LED Kuning / Oranye (Indikator Status Periodic Jamming / Waspada)
   - 1x LED Merah (Indikator Status Constant Jamming / Bahaya)
4. **3x Resistor 220Ω - 330Ω** (Resistor pembatas arus untuk proteksi LED).
5. **Breadboard & Kabel Jumper Dupont** (Male-to-Male & Male-to-Female).
6. **Kabel Micro-USB** (Penyalur daya 5V DC dan transmisi komunikasi serial UART).

<a id="2-tabel-pinout-wiring"></a>
### 2. Tabel Pinout Wiring

| Komponen | Pin Modul | Pin ESP32 DevKit V1 | Keterangan Sambungan |
| :--- | :--- | :--- | :--- |
| **OLED 0.96" SSD1306** | VCC | **3.3V / VIN** | Suplai tegangan daya 3.3V DC |
| | GND | **GND** | Ground bersama (*common ground*) |
| | SCL | **GPIO 22 (D22)** | Jalur bus I2C Clock (SCL) |
| | SDA | **GPIO 21 (D21)** | Jalur bus I2C Data (SDA) |
| **LED Hijau (Aman)** | Anoda (+) | **GPIO 25 (D25)** | Dihubungkan seri via resistor pembatas arus 220Ω - 330Ω |
| | Katoda (-) | **GND** | Ground |
| **LED Kuning (Waspada)** | Anoda (+) | **GPIO 26 (D26)** | Dihubungkan seri via resistor pembatas arus 220Ω - 330Ω |
| | Katoda (-) | **GND** | Ground |
| **LED Merah (Bahaya)** | Anoda (+) | **GPIO 27 (D27)** | Dihubungkan seri via resistor pembatas arus 220Ω - 330Ω |
| | Katoda (-) | **GND** | Ground |

<a id="3-diagram-skematik-sirkuit-ascii-art"></a>
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

<a id="status-output-logika-indikator-oled-led"></a>
<a id="status-output--logika-indikator-oled--led"></a>
## 📊 Status Output & Logika Indikator (OLED & LED)

Sistem mengelola **5 kondisi (state)** operasional:

| No | State Sistem | Masukan Serial UART | Output Layar OLED | LED Hijau (D25) | LED Kuning (D26) | LED Merah (D27) |
| :---: | :--- | :--- | :--- | :---: | :---: | :---: |
| **1** | **Splash Screen & Self-Test** | *Saat Booting / Reset TWDT / Perintah `TEST`* | Info Proyek & Status Self-Test OK | 💡 **ON** (Tes 1s) | 💡 **ON** (Tes 1s) | 💡 **ON** (Tes 1s) |
| **2** | **Status Normal** | `NORMAL` atau `1` | `[ NORMAL ]` - Status: Aman | 🟢 **ON (Solid)** | ⚪ OFF | ⚪ OFF |
| **3** | **Constant Jamming** | `CONSTANT_JAMMING` atau `2` | `! PERINGATAN BAHAYA !` - Sinyal Terblokir | ⚪ OFF | ⚪ OFF | 🔴 **ON (Solid)** |
| **4** | **Periodic Jamming** | `PERIODIC_JAMMING` atau `3` | `PERINGATAN WASPADA` - Interferensi Berkala | ⚪ OFF | 🟡 **BLINK (500ms)** | ⚪ OFF |
| **5** | **Menunggu Sinyal** | *Ketiadaan aliran data serial > 10 Detik* | `MENUNGGU SINYAL...` - Standby Host | ⚪ OFF | ⚪ OFF | ⚪ OFF |

---

<a id="1-splash-screen-self-test-pengetesan-awal-oled-led"></a>
<a id="1-splash-screen--self-test-pengetesan-awal-oled--led"></a>
### 1. Splash Screen & Self-Test (Pengetesan Awal OLED & LED)
* **Tujuan**: Memverifikasi kesiapan display grafis dan memastikan ketiga lampu indikator LED berfungsi sempurna tanpa ada jalur yang putus sebelum menerima aliran data dari host.
* **Perilaku LED**:
  1. Seluruh LED (Hijau, Kuning, Merah) menyala serentak selama 1000 ms (`LED_SELFTEST_DURATION_MS`).
  2. Seluruh LED mati sejenak.
  3. LED menyala berurutan (*running chase*): Hijau $ightarrow$ Kuning $ightarrow$ Merah $ightarrow$ Selesai.
* **Tampilan OLED (Durasi 3000 ms)**:
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

<a id="2-status-normal-aman"></a>
### 2. Status Normal (Aman)
* **Kondisi**: Sinyal maritim VHF Ch. 16 dan pelacak AIS beroperasi dalam kondisi spektrum bersih tanpa derau jamming.
* **Perilaku LED**: **LED Hijau ON (Solid)**, LED Kuning OFF, LED Merah OFF.
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

<a id="3-status-constant-jamming-bahaya"></a>
### 3. Status Constant Jamming (Bahaya)
* **Kondisi**: Terdeteksi gelombang interferensi konstan kontinu berdaya tinggi yang memblokir saluran komunikasi VHF dan AIS secara total.
* **Perilaku LED**: **LED Merah ON (Solid Bahaya)**, LED Hijau OFF, LED Kuning OFF.
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

<a id="4-status-periodic-jamming-waspada"></a>
### 4. Status Periodic Jamming (Waspada)
* **Kondisi**: Terdeteksi gelombang pengacau pulsa periodik (*pulsed interference*) yang memutus transmisi paket data AIS secara berkala.
* **Perilaku LED**: **LED Kuning berkedip ritmis (*pulsing blink*)** setiap 500 ms secara *non-blocking* (`PERIODIC_BLINK_INTERVAL`), LED Hijau OFF, LED Merah OFF.
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

<a id="5-status-menunggu-sinyal-standby-timeout-fail-safe"></a>
<a id="5-status-menunggu-sinyal-standby--timeout-fail-safe"></a>
### 5. Status Menunggu Sinyal (Standby & Timeout Fail-Safe)
* **Kondisi**: Host pemroses (Raspberry Pi/PC) belum terhubung, mengalami crash, atau tidak mengirimkan data serial selama lebih dari 10 detik (`SERIAL_TIMEOUT_MS`).
* **Tujuan**: Mencegah visualizer menampilkan status lama yang membingungkan (*stale data failure*).
* **Perilaku LED**: Seluruh LED (Hijau, Kuning, Merah) OFF dalam mode standby.
* **Tampilan OLED**:
```text
+------------------------------+
| SPECTRUM MONITOR             |
| ---------------------------- |
|       MENUNGGU SINYAL...     |
|                              |
|   [ Standby / No Data ]      |
|                              |
| ---------------------------- |
| Hubungkan Host Serial USB    |
+------------------------------+
```

---

<a id="protokol-komunikasi-data-serial"></a>
## 📡 Protokol Komunikasi Data Serial

Modul visualizer ESP32 berkomunikasi dengan komputer utama (*Raspberry Pi / PC*) melalui antarmuka serial UART via kabel USB:
* **Baud Rate**: `115200 bps`
* **Data Bits**: `8`
* **Parity**: `None`
* **Stop Bits**: `1`
* **Karakter Akhir Baris**: `
` (*Newline*) atau `
` (*CRLF*)
* **Pemrosesan String**: Case-insensitive (mendukung huruf besar maupun kecil) dengan pembersihan spasi (`.trim()`).

<a id="tabel-daftar-perintah"></a>
### Tabel Daftar Perintah

| Perintah String | Karakter Cepat | Efek pada Sistem Visualizer |
| :--- | :---: | :--- |
| `NORMAL` | `1` | Mengaktifkan Status Normal (LED Hijau ON solid, OLED: "Status: Aman") |
| `CONSTANT_JAMMING` atau `CONSTANT` | `2` | Mengaktifkan Status Constant Jamming (LED Merah ON solid, OLED peringatan bahaya) |
| `PERIODIC_JAMMING` atau `PERIODIC` | `3` | Mengaktifkan Status Periodic Jamming (LED Kuning kedip ritmis 500 ms) |
| `TEST` atau `SPLASH` | `0` | Menjalankan ulang pengujian hardware self-test dan splash screen |
| `FREEZE` | `9` | **Simulasi Crash/Hang**: Mengunci loop eksekusi secara sengaja untuk menguji keandalan auto-reboot oleh Task Watchdog Timer (TWDT) |
| `DEMO` | - | Mengaktifkan/menonaktifkan siklus demonstrasi mandiri otomatis tiap 4 detik |
| `HELP` | - | Mencetak daftar panduan seluruh perintah serial ke Serial Monitor |

---

<a id="panduan-instalasi-menjalankan"></a>
<a id="panduan-instalasi--menjalankan"></a>
## 🚀 Panduan Instalasi & Menjalankan

<a id="opsi-a-menggunakan-platformio-vs-code-direkomendasikan"></a>
<a id="opsi-a-menggunakan-platformio-vs-code---direkomendasikan"></a>
### Opsi A: Menggunakan PlatformIO (VS Code) - Direkomendasikan

1. **Buka Folder Proyek**:
   Buka VS Code, pilih **File -> Open Folder**, lalu arahkan ke:
   ```text
   D:Ù2511092\SEM III\IF316 - Mata Kuliah Pilihan 1 IoT\!Proyek\PBLIF3-MC09
   ```
2. **Kompilasi Firmware (Build)**:
   Klik ikon centang `✓` (**PlatformIO: Build**) pada status bar bawah VS Code, atau jalankan perintah CLI:
   ```bash
   pio run
   ```
3. **Unggah ke ESP32 (Upload)**:
   Hubungkan ESP32 DevKit V1 ke PC via kabel Micro-USB, lalu klik ikon panah `→` (**PlatformIO: Upload**), atau jalankan:
   ```bash
   pio run --target upload
   ```
4. **Pantau Serial Monitor**:
   Buka Serial Monitor PlatformIO pada baudrate `115200` via ikon colokan kabel, atau jalankan:
   ```bash
   pio device monitor -b 115200
   ```

---

<a id="opsi-b-menggunakan-arduino-ide"></a>
### Opsi B: Menggunakan Arduino IDE

1. Buka **Arduino IDE** (versi 2.x ke atas direkomendasikan).
2. Tambahkan URL Board ESP32 pada menu **File -> Preferences -> Additional Board Manager URLs**:
   ```text
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Buka **Tools -> Board -> Boards Manager**, cari dan instal package `esp32` by Espressif Systems.
4. Pilih board: **DOIT ESP32 DEVKIT V1**.
5. Instal dependensi pustaka via **Sketch -> Include Library -> Manage Libraries**:
   - `Adafruit SSD1306`
   - `Adafruit GFX Library`
   - `Adafruit BusIO`
6. Buka atau salin file program `src/main.cpp` serta seluruh header di folder `include/`.
7. Pilih nomor port COM yang sesuai, lalu klik **Upload**.

---

<a id="pengujian-simulasi-serial"></a>
<a id="pengujian--simulasi-serial"></a>
## 🧪 Pengujian & Simulasi Serial

<a id="1-uji-manual-via-serial-monitor"></a>
### 1. Uji Manual via Serial Monitor
Buka Serial Monitor pada baudrate `115200 bps`, pastikan opsi line ending disetel ke `Newline` atau `Both NL & CR`, lalu kirimkan perintah uji:
- Kirim `1` atau `NORMAL` $ightarrow$ LED Hijau ON Solid, OLED menampilkan kondisi Aman.
- Kirim `2` atau `CONSTANT_JAMMING` $ightarrow$ LED Merah ON Solid, OLED menampilkan Bahaya Sinyal Terblokir.
- Kirim `3` atau `PERIODIC_JAMMING` $ightarrow$ LED Kuning berkedip ritmis 500 ms, OLED menampilkan Interferensi Berkala.
- Kirim `DEMO` $ightarrow$ Sistem menjalankan transisi otomatis berulang tiap 4 detik.
- Kirim `0` atau `TEST` $ightarrow$ Sistem menjalankan ulang prosedur self-test hardware.

<a id="2-uji-otomatis-via-skrip-python"></a>
### 2. Uji Otomatis via Skrip Python
Untuk menyimulasikan output model deteksi jamming dari komputer utama (Raspberry Pi/PC), gunakan skrip Python `tools/simulate_pi.py`:

```bash
# 1. Instalasi library serial
pip install pyserial

# 2. Jalankan skrip simulator
python tools/simulate_pi.py
```
Skrip akan mendeteksi port COM ESP32 secara otomatis dan menyediakan menu interaktif pengiriman sinyal kontinu maupun pengujian berkala.

<a id="3-uji-keandalan-watchdog-timer-simulasi-freeze"></a>
### 3. Uji Keandalan Watchdog Timer (Simulasi Freeze)
Untuk memvalidasi keandalan mekanisme *hardware fail-safe*:
1. Hubungkan ESP32 dan buka Serial Monitor pada baudrate `115200`.
2. Kirim perintah `FREEZE` atau `9`.
3. Firmware akan dengan sengaja memasuki loop pengunci (*infinite blocking loop*) dan menghentikan rutinitas *feeding watchdog*.
4. **Hasil Verifikasi**: Tepat setelah **5 detik**, subsistem pengawas hardware ESP32 TWDT memicu interupsi panic:
   ```text
   E (XXXXX) task_wdt: Task watchdog got triggered. The following tasks did not reset the watchdog in time:
   E (XXXXX) task_wdt:  - loopTask (CPU 1)
   Rebooting...
   ```
5. Chip ESP32 seketika me-reboot CPU secara mandiri dan kembali ke status **Splash Screen & Self-Test** tanpa perlu tombol reset fisik ditekan oleh operator.

<a id="4-uji-timeout-fail-safe-ketiadaan-data-serial"></a>
### 4. Uji Timeout Fail-Safe Ketiadaan Data Serial
1. Kirimkan perintah status normal (`NORMAL`), amati LED Hijau menyala.
2. Hentikan pengiriman data serial dan tunggu selama **10 detik**.
3. **Hasil Verifikasi**: Sistem secara otomatis mendeteksi ketiadaan sinyal host (`SERIAL_TIMEOUT_MS`), mematikan LED, dan memperbarui layar OLED menjadi `MENUNGGU SINYAL... [Standby / No Data]`.

---

<a id="identitas-proyek-tim-pembagian-tugas"></a>
<a id="identitas-proyek-tim--pembagian-tugas"></a>
## 👥 Identitas Proyek, Tim & Pembagian Tugas

<a id="1-identitas-akademik"></a>
### 1. Identitas Akademik
* **Institusi**: Politeknik Negeri Batam
* **Jurusan / Program Studi**: Jurusan Teknik Informatika / Sarjana Terapan Rekayasa Perangkat Lunak (D4 RPL)
* **Semester / Kelas**: Semester III (Ganjil 2026/2027) / IF Malam C
* **Mata Kuliah Terintegrasi**:
  - **IF316 - Mata Kuliah Pilihan 1 IoT**
  - **IF314 - Proyek Inovasi Agile**
  - **IF315 - Rekayasa Perangkat Lunak Lanjut**
  - **IF318 - Statistika**
* **Kode Kelompok**: **PBLIF3-MC09**

<a id="2-anggota-tim-pembagian-tugas-task-assignment"></a>
<a id="2-anggota-tim--pembagian-tugas-task-assignment"></a>
### 2. Anggota Tim & Pembagian Tugas (Task Assignment)

| No | Nama Mahasiswa | NIM | Peran Pengembang | Rincian Tugas & Modul yang Dikembangkan |
| :---: | :--- | :---: | :---: | :--- |
| **1** | **Galeh Wibisono** | `3312511092` | **Pengembang 1** | **[T-09] Pemrograman Boot Splash Screen, Self-Test LED & Status Normal**:<br>• Perancangan tata letak grafis boot splash screen (`SplashScreen.h/.cpp`).<br>• Prosedur pengujian startup hardware 3 LED (`LedController.h/.cpp`).<br>• Visualisasi kondisi spektrum Normal/Aman pada OLED (`NormalStatus.h/.cpp`).<br><br>**[T-11] Proteksi Watchdog Timer & Fail-Safe Serial**:<br>• Implementasi Task Watchdog Timer (TWDT 5 detik) berbasis `<esp_task_wdt.h>`.<br>• Rutinitas non-blocking feeding watchdog pada `loop()`.<br>• Penanganan fail-safe timeout serial ketiadaan data 10 detik (`STATE_WAITING`).<br>• Perancangan skenario uji auto-reboot simulasi hang (`FREEZE`).<br>• Penyusunan konfigurasi tunggal hardware (`include/config.h`). |
| **2** | **Muhamad Abid Al Mubarok** | `3312511099` | **Pengembang 2** | **[T-10] Pemrograman Visualisasi Constant Jamming & Periodic Jamming**:<br>• Perancangan antarmuka visual peringatan bahaya *Constant Jamming* pada OLED + aktivasi solid LED Merah (`ConstantJammingStatus.h/.cpp`).<br>• Perancangan antarmuka visual peringatan waspada *Periodic Jamming* pada OLED + kedip ritmis non-blocking LED Kuning 500 ms (`PeriodicJammingStatus.h/.cpp`).<br><br>**Pengujian & Integrasi**:<br>• Pengujian parsing komunikasi data serial UART bersama pengembang 1.<br>• Validasi interoperabilitas antarmuka class dengan controller utama. |

<a id="3-stakeholder-pembimbing"></a>
<a id="3-stakeholder--pembimbing"></a>
### 3. Stakeholder & Pembimbing
* **Pengusul Proyek**: Metta Santiputri, S.T., M.Sc., Ph.D.
* **Klien / Product Owner**: Lettisia Nurdayenti
* **Manajer Proyek (Manpro)**: Nur Cahyono Kurniawan, S.T., M.Kom., Ph.D.

---

<a id="lisensi"></a>
## 📄 Lisensi

Dokumentasi dan kode sumber proyek ini dikembangkan untuk pemenuhan tugas akademik Project-Based Learning (PBL) dan penelitian keselamatan navigasi spektrum maritim di lingkungan **Politeknik Negeri Batam**. Seluruh hak cipta dilindungi undang-undang.
