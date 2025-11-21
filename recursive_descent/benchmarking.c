#include "jajson.h"
#include <sys/time.h>

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
    long fileSize =ftello(file);
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

int main() {
	struct timeval start_time, end_time;
	double elapsed_time;

	long* file_size;
	char* file_contents = readFile("../benchmarking/generate/gened_output.json", file_size);

	double time_sum;
	int times = 20;

	for (int i = 0; i < times; i++) {
		gettimeofday(&start_time, NULL);

        // DO something
        json_value_t *loaded_json = load_json(file_contents);

        gettimeofday(&end_time, NULL);

        elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0;
        elapsed_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0;

        printf("Parsed %ld bytes in %lf ms\n", *file_size, elapsed_time);
        time_sum += elapsed_time;
        free_json(loaded_json);
	}

    printf("Average parsing throughput: %lf\n", time_sum / (double) times);
    return 0;
}