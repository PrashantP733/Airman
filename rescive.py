# uart_rx.py
import serial
import csv
import time

PORT = "/dev/ttyUSB0"
BAUD = 115200
OUT_FILE = "output.csv"

def calc_checksum(s):
    chk = 0
    for c in s:
        chk ^= ord(c)
    return chk

ser = serial.Serial(PORT, BAUD, timeout=1)

with open(OUT_FILE, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow([
        "timestamp","ax","ay","az","gx","gy","gz","alt","temp"
    ])

    print("Listening for telemetry...\n")

    while True:
        line = ser.readline().decode(errors="ignore").strip()
        if not line.startswith("$") or "*" not in line:
            continue

        body, rx_chk = line[1:].split("*")
        rx_chk = int(rx_chk, 16)

        if calc_checksum(body) != rx_chk:
            print("Checksum error")
            continue

        fields = body.split(",")
        if fields[0] != "L1":
            continue

        data = fields[1:]
        writer.writerow(data)
        f.flush()

        print(
            f"t={data[0]} | "
            f"A=({data[1]}, {data[2]}, {data[3]}) "
            f"G=({data[4]}, {data[5]}, {data[6]}) "
            f"Alt={data[7]}m Temp={data[8]}C"
        )
