import sys
import csv

def valid_sum(frame):
    if not frame.startswith('$') or '*' not in frame:
        return False

    try:
        data, recv_chk = frame[1:].split('*')
        calc_chk = 0
        for ch in data:
            calc_chk ^= ord(ch)

        return calc_chk == int(recv_chk, 16)
    except:
        return False


with open("output.csv", "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow([
        "timestamp_ms", "ax", "ay", "az",
        "gx", "gy", "gz", "alt", "temp"
    ])

    print("Python receiver started...\n")

    for line in sys.stdin:
        line = line.strip()
        if valid_sum(line):
            payload = line[1:line.index('*')]
            fields = payload.split(',')

            if fields[0] == "L1":
                writer.writerow(fields[1:])
                print(
                    f"Time={fields[1]} ms | "
                    f"Ax={fields[2]} Ay={fields[3]} Az={fields[4]} | "
                    f"Gx={fields[5]} Gy={fields[6]} Gz={fields[7]} | "
                    f"Alt={fields[8]} Temp={fields[9]}"
                )
        else:
            print("Invalid frame:", line)
