#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "v2.h"

int main(int argc, char *argv[])
{
	Sudoku *s;
	FILE* fp;
	int i, j, m, n, files, fileFlag = 0, checkFlag = 0;
	char* filename;
	char buffer[100];

	files = argc;
		if (files < 2)
		{
			fileFlag = 1;
			printf("Enter file name:\n");
			fgets(buffer, 100, stdin);
			filename = strtok(buffer, "\n");
			fp = fopen(filename, "r");
			files = 2;
		}

	s = mmap(NULL, sizeof(Sudoku), PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (s == MAP_FAILED)
		return -1;

	for (n = 1; n < files; ++n)
	{
		//Read files to sudoko s
		if(fileFlag == 0)
		{
			fp = fopen(argv[n], "r");
			filename = argv[n];
		}
		if (fp)
		{
			for (i = 0; i < ROWS * COLS; i++)
				fscanf(fp, "%d", &s->fields[i]);
			for (j = 0; j < 3; ++j)
				s->status[j] = 0;
		} else
		{
			printf("Failed to open the file\n");
			return 0;
		}
		for (m = 0; m < 3; ++m)
		{
			switch (fork()) /* Parent and child share mapping */
			{
			case -1:
				return 0;

			case 0: /* Child: increment shared integer and exit */
				decisionMaker(s, m);
				if (munmap(s, sizeof(int)) == -1)
					return 0;
				return 0;
				//return 0;
			}
			if (wait(NULL) == -1)
				return 0;
		}

		checkFlag = 0;
		for (i = 0; i < 3; ++i)
		{
			if (s->status[i] != 1)
			{
				checkFlag = 1;
				printf("%s is not legal\n", filename);
				break;
			}
		}
		if(checkFlag == 0)
			printf("%s is legal\n", filename);
	}

	if (munmap(s, sizeof(int)) == -1)
		return 0;

}

void decisionMaker(Sudoku *s, int i)
{
	switch (i)
	{
	case 0:
		checkRows(s);
		return;
	case 1:
		checkCols(s);
		return;
	case 2:
		checkBoxes(s);
		return;
	}
}

void checkRows(Sudoku *s)
{
	int i, j;
	int check[9] = { 0 };

	for (i = 0; i < ROWS; ++i)
	{
		for (j = 0; j < COLS; ++j)
		{
			if (s->fields[i * ROWS + j] >= 1 && s->fields[i * ROWS + j] <= 9)
			{
				check[(s->fields[i * ROWS + j]) - 1] = 1;
			} else
			{
				s->status[0] = -1;
				return; //faulty information
			}
		}
		for (j = 0; j < 9; ++j)
		{
			if (check[j] != 1)
			{
				s->status[0] = 0;
				return;
			}
			//add return value through pipe. FD - FAILURE
		}
		for (j = 0; j < 9; ++j)
			check[j] = 0;
	}
	s->status[0] = 1;
}

void checkCols(Sudoku *s)
{
	int i, j;
	int check[9] = { 0 };

	for (i = 0; i < ROWS; ++i)
	{
		for (j = 0; j < COLS; ++j)
		{
			if (s->fields[i + j * COLS] >= 1 && s->fields[i + j * COLS] <= 9)
				check[s->fields[i + j * COLS] - 1] = 1;
			else
			{
				s->status[1] = -1;
				return;
			}
		}
		for (j = 0; j < COLS; ++j)
		{
			if (check[j] != 1)
			{
				s->status[1] = 0;
				return;
			}
		}
	}
	s->status[1] = 1;
}

void checkBoxes(Sudoku *s)
{
	int i, j, k;
	int check[BOX * BOX] = { 0 };
	int mult = 0;

	for (i = 0; i < 9; ++i)
	{
		if (i >= 3 && i <= 5)
			mult = 18;
		else if (i >= 6 && i <= 8)
			mult = 36;

		for (j = 0; j < BOX; ++j)
		{
			for (k = 0; k < BOX; ++k)
			{
				if (s->fields[i * BOX + mult + ROWS * j + k] >= 1
						&& s->fields[i * BOX + mult + ROWS * j + k] <= 9)
					check[s->fields[i * BOX + mult + ROWS * j + k] - 1] = 1;
				else
				{
					s->status[2] = -1;
					return; //invalid character
				}
			}
		}
		for (j = 0; j < COLS; ++j)
		{
			if (check[j] != 1)
			{
				s->status[2] = 0;
				return; //incorrect Sudoku
			}
		}
		for (j = 0; j < 9; ++j)
			check[j] = 0;
	}
	s->status[2] = 1; //valid solution
}

