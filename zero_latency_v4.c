#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

// CUSTOM FAST-FLOAT PARSER
// Skips all the overhead of atof()
static inline double fast_atof(char *p) {
    double res = 0, fraction = 0;
    int has_fraction = 0;
    double divisor = 10.0;

    while (*p >= '0' && *p <= '9') {
        res = res * 10.0 + (*p - '0');
        p++;
    }
    if (*p == '.') {
        p++;
        while (*p >= '0' && *p <= '9') {
            res += (*p - '0') / divisor;
            divisor *= 10.0;
            p++;
        }
    }
    return res;
}

int main() {
    const char *filename = "massive_server_logs.csv";
    int fd = open(filename, O_RDONLY);
    if (fd == -1) { perror("Error opening file"); return 1; }

    struct stat sb;
    fstat(fd, &sb);

    char *addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) { perror("mmap"); return 1; }

    printf("🔥 God-Mode Engine V4.0 (Custom Parser) Active...\n");
    clock_t start = clock();

    char *ptr = addr;
    char *end = addr + sb.st_size;
    long double total_latency = 0;
    unsigned long long count = 0;

    // Skip Header
    while (ptr < end && *ptr != '\n') ptr++;
    ptr++; 

    // Optimized Scanning
    while (ptr < end) {
        int commas = 0;
        // Jump to 3rd column
        while (ptr < end && commas < 2) {
            if (*ptr == ',') commas++;
            ptr++;
        }

        // Parse latency_ms using our custom Fast-Float
        total_latency += fast_atof(ptr);
        count++;

        // Skip to next line
        while (ptr < end && *ptr != '\n') ptr++;
        ptr++;
    }

    clock_t stop = clock();
    double time_spent = (double)(stop - start) / CLOCKS_PER_SEC;

    printf("\n--- GOD-MODE REPORT ---\n");
    printf("✅ Rows: %llu\n", count);
    printf("📊 Avg: %.2Lf ms\n", total_latency / count);
    printf("⏱️ Final Time: %.4f seconds\n", time_spent);
    printf("-----------------------\n");

    munmap(addr, sb.st_size);
    close(fd);
    return 0;
}
