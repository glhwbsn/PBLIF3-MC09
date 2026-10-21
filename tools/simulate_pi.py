#!/usr/bin/env python3
"""
tools/simulate_pi.py
====================
Simulator Serial UART — Host Komputer (PC / Raspberry Pi) -> ESP32 Visualizer
Proyek : PBLIF3-MC09 — Visualisasi Status Deteksi Jamming Spektrum Maritim
Target : ESP32 DevKit V1

Fitur:
  - Deteksi otomatis port serial USB ESP32 (CP210x, CH340, FTDI, dll.)
  - Thread pembaca respon serial ESP32 secara real-time
  - 3 Pilihan Menu Pengujian Status:
      [1] NORMAL (Status Aman -> LED Hijau Solid, OLED: Aman)
      [2] CONSTANT JAMMING (Bahaya -> LED Merah Solid, OLED: Bahaya)
      [3] PERIODIC JAMMING (Waspada -> LED Kuning Kedip 500ms, OLED: Waspada)
      [0] Keluar (Exit)
"""

import sys
import time
import threading
import argparse

try:
    import serial
    import serial.tools.list_ports
except ImportError:
    print("\n[ERROR] Library 'pyserial' belum terinstal di lingkungan Python ini.")
    print("Silakan jalankan perintah berikut terlebih dahulu:")
    print("    pip install pyserial\n")
    sys.exit(1)

DEFAULT_BAUDRATE = 115200


def list_serial_ports():
    """Mendeteksi dan mengembalikan daftar port serial yang tersedia."""
    return serial.tools.list_ports.comports()


def auto_detect_esp32_port(ports):
    """Mencari port yang kemungkinan besar adalah ESP32 (berdasarkan deskripsi umum)."""
    esp_keywords = ["CP210", "CH340", "USB-SERIAL", "FTDI", "UART", "ESP32"]
    for p in ports:
        desc = (p.description or "") + " " + (p.manufacturer or "")
        for kw in esp_keywords:
            if kw.lower() in desc.lower():
                return p.device
    return None


def serial_reader_thread(ser, stop_event):
    """Thread latar belakang untuk mencetak data masuk dari ESP32."""
    while not stop_event.is_set():
        try:
            if ser.in_waiting > 0:
                line = ser.readline().decode("utf-8", errors="replace")
                if line:
                    print(f"  [ESP32] {line.rstrip()}")
        except Exception:
            break
        time.sleep(0.02)


def send_command(ser, cmd_str):
    """Mengirim string perintah berakhiran newline ke ESP32."""
    payload = (cmd_str.strip() + "\n").encode("utf-8")
    ser.write(payload)
    ser.flush()
    print(f"\n>>> [HOST TERKIRIM] '{cmd_str.strip()}'")


def print_banner(port, baudrate):
    print("=" * 65)
    print(" 🛰️  PBLIF3-MC09 — UART SIMULATOR HOST (PC/Raspberry Pi)")
    print(f" Port Terhubung : {port} | Baudrate: {baudrate} bps")
    print("=" * 65)


def print_menu():
    print("\n" + "=" * 52)
    print(" PILIHAN FITUR STATUS:")
    print("   [1] NORMAL")
    print("       -> Status Aman (LED Hijau Solid, OLED: Aman)")
    print("   [2] CONSTANT JAMMING")
    print("       -> Status Bahaya (LED Merah Solid, OLED: Bahaya)")
    print("   [3] PERIODIC JAMMING")
    print("       -> Status Waspada (LED Kuning Kedip 500ms)")
    print("   [4] SIMULASI FREEZE / UJI WATCHDOG")
    print("       -> Uji Crash/Hang (Auto-Reboot dalam 5 detik)")
    print("   [0] Keluar (Exit)")
    print("=" * 52)


def main():
    parser = argparse.ArgumentParser(description="Simulator UART Host untuk ESP32 Visualizer (PBLIF3-MC09)")
    parser.add_argument("-p", "--port", type=str, default=None, help="Nama port serial (misal COM7 atau /dev/ttyUSB0)")
    parser.add_argument("-b", "--baud", type=int, default=DEFAULT_BAUDRATE, help=f"Baudrate serial (default: {DEFAULT_BAUDRATE})")
    parser.add_argument("-c", "--cmd", type=str, default=None, help="Kirim satu perintah langsung lalu keluar (one-shot mode)")
    args = parser.parse_args()

    # 1. Deteksi Port
    target_port = args.port
    ports = list_serial_ports()

    if not target_port:
        if not ports:
            print("\n[ERROR] Tidak ada port COM / serial yang terdeteksi.")
            print("Pastikan kabel USB ESP32 sudah terhubung ke PC/laptop.")
            sys.exit(1)

        detected = auto_detect_esp32_port(ports)
        if detected:
            print(f"[INFO] Port ESP32 terdeteksi otomatis: {detected}")
            target_port = detected
        elif len(ports) == 1:
            target_port = ports[0].device
            print(f"[INFO] Menggunakan port serial tunggal: {target_port}")
        else:
            print("\nDaftar port serial yang terdeteksi:")
            for idx, p in enumerate(ports, start=1):
                print(f"  [{idx}] {p.device} - {p.description}")
            choice = input(f"\nPilih nomor port (1-{len(ports)}): ").strip()
            try:
                sel = int(choice) - 1
                if 0 <= sel < len(ports):
                    target_port = ports[sel].device
                else:
                    print("[ERROR] Pilihan tidak valid.")
                    sys.exit(1)
            except ValueError:
                print("[ERROR] Masukkan angka pilihan yang benar.")
                sys.exit(1)

    # 2. Buka Koneksi Serial
    try:
        print(f"\n[INFO] Menghubungkan ke {target_port} pada {args.baud} bps...")
        ser = serial.Serial(target_port, args.baud, timeout=1.0)
        time.sleep(1.5)  # Toleransi inisialisasi DTR
    except Exception as e:
        print(f"\n[ERROR] Gagal membuka port {target_port}: {e}")
        print("Pastikan port tidak sedang dibuka oleh Serial Monitor PlatformIO atau aplikasi lain.")
        sys.exit(1)

    # 3. Jalankan thread pembaca log dari ESP32
    stop_event = threading.Event()
    reader = threading.Thread(target=serial_reader_thread, args=(ser, stop_event), daemon=True)
    reader.start()

    print_banner(target_port, args.baud)

    # 4. Mode One-Shot jika argumen --cmd diberikan
    if args.cmd:
        send_command(ser, args.cmd)
        time.sleep(1.0)
        stop_event.set()
        ser.close()
        print("[INFO] Perintah terkirim. Selesai.")
        return

    # 5. Mode Menu Interaktif
    try:
        while True:
            print_menu()
            pilihan = input("Masukkan pilihan Anda [1-4] atau [0] untuk keluar: ").strip()

            if pilihan == "1":
                send_command(ser, "NORMAL")
            elif pilihan == "2":
                send_command(ser, "CONSTANT")
            elif pilihan == "3":
                send_command(ser, "PERIODIC")
            elif pilihan == "4":
                send_command(ser, "FREEZE")
            elif pilihan in ("0", "q", "Q", "exit"):
                print("\n[INFO] Menutup koneksi serial dan keluar...")
                break
            else:
                print("[WARN] Pilihan tidak valid. Silakan pilih 1, 2, 3, 4, atau 0.")

            time.sleep(0.5)

    except KeyboardInterrupt:
        print("\n\n[INFO] Program dihentikan oleh pengguna (Ctrl+C).")
    finally:
        stop_event.set()
        if ser.is_open:
            ser.close()
        print("[OK] Port serial telah ditutup dengan aman.")


if __name__ == "__main__":
    main()
