/**
  * SO, 2016
  * Lab #5
  *
  * Task #3, lin
  *
  * Valgrind usage
  */
#include <stdio.h>
#include <stdlib.h>

#define MAX		100
#define CHUNK		10
#define SEED		23
#define STOP		17


int main(void)
{
	int nr, count = 0;
	int *buffer;

	buffer = malloc(CHUNK * sizeof(int));

	if (buffer == NULL) {
		perror("malloc");
		exit(-1);
	}

	srand(SEED);
	do {
		nr = 1 + rand() % MAX;

		buffer[count] = nr;
		count++;

		if (count % CHUNK == 0) {
			printf("Realloc\n");
			buffer = realloc(buffer, (count + CHUNK) * sizeof(int));

			if (buffer == NULL) {
				perror("realloc");
				exit(-1);
			}
		}

		printf("Nr %d - val %d\n", count - 1, buffer[count - 1]);

	} while (buffer[count - 1] != STOP);

	/* TODO - eliberare memorie */
	free(buffer);

	return 0;
}
