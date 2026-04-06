#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main() {
    const char *filename = "massive_server_logs.csv";
    int fd = open(filename, O_RDONLY);
    if (fd == -1) { perror("Error opening file"); return 1; }

    // Get file size
    struct stat sb;
    if (fstat(fd, &sb) == -1) { perror("fstat"); return 1; }

    // Memory Map the file
    char *addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) { perror("mmap"); return 1; }

    printf("⚡ Zenith Engine V3.0 (mmap) Active...\n");
    clock_t start = clock();

    char *ptr = addr;
    char *end = addr + sb.st_size;
    long double total_latency = 0;
    unsigned long long count = 0;

    // Skip Header line
    while (ptr < end && *ptr != '\n') ptr++;
    ptr++; 

    // Raw Pointer Parsing (Manual State Machine)
    while (ptr < end) {
        int comma_count = 0;
        // We need the 3rd column (index 2)
        while (ptr < end && comma_count < 2) {
            if (*ptr == ',') comma_count++;
            ptr++;
        }

        // We are at the start of latency_ms
        char *start_num = ptr;
        while (ptr < end && *ptr != ',') ptr++;
        
        // Convert substring to double manually
        total_latency += atof(start_num);
        count++;

        // Skip to end of line
        while (ptr < end && *ptr != '\n') ptr++;
        ptr++;
    }

    clock_t stop = clock();
    double time_spent = (double)(stop - start) / CLOCKS_PER_SEC;

    printf("\n--- ZENITH REPORT ---\n");
    printf("✅ Processed Rows: %llu\n", count);
    if (count > 0) printf("📊 Average Latency: %.2Lf ms\n", total_latency / count);
    printf("⏱️ Zenith Time: %.4f seconds\n", time_spent);
    printf("---------------------\n");

    munmap(addr, sb.st_size);
    close(fd);
    return 0;
}
