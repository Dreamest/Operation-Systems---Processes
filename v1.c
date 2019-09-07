#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <string.h>
#include "v1.h"

int main(int argc, char* argv[])
{
	int i, m, result, files, fileFlag = 0, flag = 0, wv = 0;
	int fd[4][2];
	char buffer1[100], buffer2[100];
	char* filename;
	Sudoku s;
	FILE *fp;

	files = argc;
		if (files < 2)
		{
			fileFlag = 1;
			printf("Enter file name:\n");
			fgets(buffer2, 100, stdin);
			filename = strtok(buffer2, "\n");
			fp = fopen(filename, "r");
			files = 2;
		}

	for (m = 1; m < files; ++m)
	{ // Loops over files in argv
		for (i = 0; i < 4; ++i)
		{ // create pipes. 0-2 parent->child, 3 children->parent
			if (pipe(fd[i]) < 0)
			{
				fprintf(stderr, "pipe failed");
				return -1;
			}
		}
		if(fileFlag == 0)
		{
			fp = fopen(argv[m], "r");
			filename = argv[m];
		}
		if (fp)
		{
			for (i = 0; i < ROWS * COLS; i++)
				fscanf(fp, "%d", &s.fields[i]);
			for (i = 0; i < 3; ++i)
				s.status[i] = 0;
		} else
		{
			printf("Failed to open file.\n");
			return 0;
		}

		for (i = 0; i < 3; ++i) //process creation
		{
			if (fork() == 0)
			{
				close(fd[i][1]);
				close(fd[3][0]);
				dup2(fd[i][0], STDIN_FILENO);
				dup2(fd[3][1], STDOUT_FILENO);
				sprintf(buffer1, "%d", i);
				char *input[] = { file, buffer1, NULL };
				execve(file, input, NULL);
				printf("Child number %d failed executing.\n", (i+1));
				return -1;
			}
			close(fd[i][0]);
		}
		close(fd[3][1]);

		// Writes data to pipes used by children
		wv = 0;
		for (i = 0; i < 3; ++i)
		{
			wv = write(fd[i][1], &s, sizeof(Sudoku));
			if (wv != sizeof(Sudoku))
			{
				printf("Error writing to pipe.\n");
				return -1;
			}
		}
		//read results from children, place them in struct.
		for (i = 0; i < 3; ++i)
		{
			read(fd[3][0], &result, sizeof(int));
			if (result == 11 || result == 10 || result == -1)
				s.status[0] = result;
			if (result == 21 || result == 20 || result == -2)
				s.status[1] = result;
			if (result == 31 || result == 30 || result == -3)
				s.status[2] = result;
		}

		// check results.
		flag = 0;
		for (i = 0; i < 3; ++i)
		{
			if ((s.status[i] % 10 == 0) || (s.status[i] < 0))
			{
				if (flag == 0)
				{
					printf("%s is not legal\n", filename);
					flag = 1;
					break;

				}
			}
		}
		if (flag == 0)
			printf("%s is legal\n", filename);
	}
	return 1;
}

