#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#ifndef TXT
#define TXT 0
#else
#undef TXT
#define TXT 1
#endif

int main (int argc, char *argv[]) {
	int i, j, N = argc > 1 ? atoi (argv[1]) : 32;
	char** arr = malloc (N * sizeof (char*));
	for (i = 0; i < N; i++)
		arr[i] = malloc (N * sizeof (char));

	srand(time(NULL));
	for (i = 0; i < N; i++)
		for (j = i; j < N; j++)
			arr[j][i] = arr[i][j] = j == i ? 0 : rand() % 2;

	FILE *fp = TXT ? fopen ("graph.txt", "w") : stdout;
	for (i = 0; i < N; i++)
		for (j = 0, fprintf (fp, "%s%d", i ? "\n" : "", i + 1); j < N; j++)
			if (arr[i][j])
				fprintf (fp, " %d", j + 1);

	fputc ('\n', fp);
	fclose (fp);
	for (i = 0; i < N; i++)
		free (arr[i]);
	free (arr);

	return 0;
}
