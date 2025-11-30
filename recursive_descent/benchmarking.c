#include "jajson.h"
#include <sys/time.h>
#include <limits.h>


char *readFile(const char *filename, long *filesize) 
{
    // Open file
    FILE* file = fopen(filename, "r");

    if (file == NULL) 
    {
        perror("Error opening file");
        return NULL;
    }

    // Get file size
    fseeko(file, 0L, SEEK_END);
    long fileSize = ftello(file);
	*filesize = fileSize;
    fseeko(file, 0L, SEEK_SET);

    // Allocate memory
    char *buffer = (char*) malloc(fileSize + 1); // + 1 for the null character
    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    // Read string into buffer
    size_t bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead != fileSize) {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    // Set null terminator
    buffer[fileSize] = '\0';
    fclose(file);
    return buffer;
}

void benchmark_throughput() {
    struct timeval start_time, end_time;
	double elapsed_time;

	long* file_size = (long *) malloc(sizeof(long)); // make sure to allocate heap memory to pointer
	char* file_contents = readFile("../benchmark_generation/generate/gened_output.json", file_size);

	double time_sum;
    double best_time = INT_MAX;
	int times = 20;

	for (int i = 0; i < times; i++) {
		gettimeofday(&start_time, NULL);

        // DO something
        json_value_t *loaded_json = load_json(file_contents);

        gettimeofday(&end_time, NULL);

        elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0;
        elapsed_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0;

        if (elapsed_time < best_time) {
            best_time = elapsed_time;
        }

        printf("Parsed %ld bytes in %lf ms\n", *file_size, elapsed_time);
        time_sum += elapsed_time;
        free_json(loaded_json);
	}

    free(file_size);
    free(file_contents);
    printf("Average parsing throughput: %lf\n", time_sum / (double) times);
    printf("Best parse time: %lf\n", best_time);
}

// Define ReaD Time Stamp Counter  (RDTSC) instructions for benchmarking cycles 
/**
 * Commentary on what instructions do:
 * - CPUID => forces processor to run this command, asks processor what features it supports
 * - RDTSC => reads the processor's 64 bit cycle counter 
 *              => usually split into 2 32 bit chunks; high bits go into EDX register; low bits go into EAX register
 */
#define RDTSC_START(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "cpuid\n\t"                                                       \
            "rdtsc\n\t"                                                       \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)

#define RDTSC_FINAL(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "rdtscp\n\t"                                                      \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            "cpuid\n\t"                                                       \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)

void benchmark_cycles() {
    uint64_t cycles_start, cycles_final, cycles_diff, min_diff;

	long* file_size = (long *) malloc(sizeof(long)); // make sure to allocate memory
	char* file_contents = readFile("../benchmark_generation/generate/gened_output.json", file_size);

	int times = 20;

	for (int i = 0; i < times; i++) {
        RDTSC_START(cycles_start);

        // DO something
        json_value_t *loaded_json = load_json(file_contents);

        RDTSC_FINAL(cycles_final);

        cycles_diff = cycles_final - cycles_start;
        if (cycles_diff < min_diff) {
            min_diff = cycles_diff;
        }

        printf("Cycles elapsed: %llu\n", cycles_diff);
        free_json(loaded_json);
	}

    free(file_size);
    free(file_contents);
    float per_byte = min_diff / (double) *file_size;
    printf("%.2f cycles per byte", per_byte);
    printf("\n");
}

int main() {
    benchmark_throughput();

    benchmark_cycles();

    return 0;
}