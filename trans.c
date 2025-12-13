// telemetry_tx.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define UART_DEV "/dev/ttyUSB0"
#define BAUD B115200

unsigned char calc_checksum(const char *s)
{
    unsigned char chk = 0;
    while (*s) chk ^= (unsigned char)(*s++);
    return chk;
}

long timestamp_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (ts.tv_sec * 1000L) + (ts.tv_nsec / 1000000);
}

int main()
{
    int fd = open(UART_DEV, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("UART open failed");
        return 1;
    }

    struct termios tty;
    tcgetattr(fd, &tty);
    cfsetospeed(&tty, BAUD);
    cfsetispeed(&tty, BAUD);
    tty.c_cflag = CS8 | CREAD | CLOCAL;
    tty.c_iflag = tty.c_oflag = tty.c_lflag = 0;
    tcsetattr(fd, TCSANOW, &tty);

    double t = 0.0;

    while (1) {
        long ts = timestamp_ms();

        double ax = sin(t);
        double ay = cos(t);
        double az = 9.8 + 0.05 * sin(2*t);

        double gx = 10 * sin(t);
        double gy = 10 * cos(t);
        double gz = 5 * sin(0.5*t);

        double alt  = 100.0 + 2.0 * sin(0.2*t);
        double temp = 25.0 + 1.0 * cos(0.1*t);

        char payload[256];
        snprintf(payload, sizeof(payload),
            "L1,%ld,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
            ts, ax, ay, az, gx, gy, gz, alt, temp);

        unsigned char chk = calc_checksum(payload);

        char frame[300];
        snprintf(frame, sizeof(frame), "$%s*%02X\r\n", payload, chk);

        write(fd, frame, strlen(frame));

        t += 0.05;
        usleep(50000); // 20 Hz
    }

    close(fd);
    return 0;
}
