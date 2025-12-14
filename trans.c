#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#define FRAME_INTERVAL_MS 50   // 20 Hz

unsigned char check_sum(const char *data)
{
    unsigned char chk = 0;
    while (*data)
    {
        chk ^= *data;
        data++;
    }
    return chk;
}

int main()
{
    unsigned long times = 0;
    char arr[256];
    unsigned char sum;

    srand(time(NULL));

    while (1)
    {
        // Simulated sensor data
        float ax = sin(times / 1000.0) * 2.0;
        float ay = cos(times / 1000.0) * 2.0;
        float az = 9.8 + ((rand() % 100) / 100.0 - 0.5);

        float bx = ((rand() % 200) / 100.0 - 1.0);
        float by = ((rand() % 200) / 100.0 - 1.0);
        float cz = ((rand() % 200) / 100.0 - 1.0);

        float alt = 100.0 + sin(times / 5000.0) * 5.0;
        float temp = 25.0 + cos(times / 5000.0) * 0.5;

        // Build arr 
        snprintf(arr, sizeof(arr),
                 "L1,%lu,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
                 times, ax, ay, az, bx, by, cz, alt, temp);

        // Calculate check_sum
        sum = check_sum(arr);

        // Output full frame (UART simulation)
        printf("$%s*%02X\n", arr, sum);
        fflush(stdout);

        usleep(FRAME_INTERVAL_MS * 1000);
        times = times + FRAME_INTERVAL_MS;
    }

    return 0;
}
