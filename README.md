# Level 1 – Telemetry Pipeline

## Build & Run
### Transmitter
gcc telemetry_tx.c -o telemetry_tx -lm
./telemetry_tx

### Receiver
pip install pyserial
python3 uart_rx.py

## Frame Rate
20 Hz (50 ms per frame)

## Assumptions
- Linux UART (/dev/ttyUSB0)
- Simulated sensor data
- No packet loss handling beyond checksum

## AI Tools Used
- ChatGPT for code structure and checksum logic
- Manual testing and validation

## Simplifications
- Fixed baud rate
- No dynamic device discovery
- No binary protocol (ASCII only)
