#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define BUFFER_SIZE 65536 // Increased to 64KB for better throughput

int main() {
    const char *filename = "massive_server_logs.csv";
    FILE *file = fopen(filename, "r");
    
    // 1. ROBUST FILE CHECK
    if (!file) {
        fprintf(stderr, "❌ FATAL: Could not open %s. Reason: %s\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    long double total_latency = 0;
    unsigned long long count = 0;
    unsigned long long errors = 0;
    unsigned long long line_num = 0;

    printf("🛡️ Hardened C-Engine V2.0 Active...\n");
    clock_t start = clock();

    // 2. SAFE HEADER SKIP
    if (!fgets(buffer, sizeof(buffer), file)) {
        fprintf(stderr, "❌ FATAL: File is empty or corrupt.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // 3. THE HARDENED LOOP
    while (fgets(buffer, sizeof(buffer), file)) {
        line_num++;
        
        // Manual Parsing (Faster than sscanf)
        char *token;
        char *rest = buffer;
        int column = 0;
        double val = 0;

        // We only care about the 3rd column (latency_ms)
        while ((token = strtok_r(rest, ",", &rest))) {
            if (column == 2) { // 0-indexed: log_id(0), user_id(1), latency_ms(2)
                char *endptr;
                val = strtod(token, &endptr);
                
                // VALIDATION: Did we actually get a number?
                if (token == endptr) {
                    errors++;
                } else {
                    total_latency += val;
                    count++;
                }
                break; 
            }
            column++;
        }
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    // 4. OBSERVABILITY OUTPUT
    printf("\n--- EXECUTION REPORT ---\n");
    printf("✅ Processed Rows: %llu\n", count);
    printf("⚠️ Skipped Errors: %llu\n", errors);
    if (count > 0) {
        printf("📊 Average Latency: %.2Lf ms\n", total_latency / count);
    }
    printf("⏱️ Total Time: %.2f seconds\n", time_spent);
    printf("------------------------\n");

    fclose(file);
    return EXIT_SUCCESS;
}
