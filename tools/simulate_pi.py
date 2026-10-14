#!/usr/bin/env python3
"""
======================================================================================
SKRIP SIMULATOR SERIAL PENGIRIM STATUS JAMMING (RASPBERRY PI / PC TO ESP32)
Proyek PBLIF3-MC09: Visualisasi Status Deteksi Jamming Spektrum Maritim
======================================================================================
Deskripsi:
Skrip Python ini berfungsi menyimulasikan output model klasifikasi deteksi jamming
yang berjalan di komputer utama (Raspberry Pi/PC) dengan mengirimkan string status:
  - "NORMAL"
  - "CONSTANT_JAMMING"
  - "PERIODIC_JAMMING"
melalui komunikasi Serial UART (USB) ke mikrokontroler ESP32 visualizer.

Penggunaan:
  python simulate_pi.py
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
                # Kirim teks kustom langsung
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
