/*
 * rows.c
 *
 *  Created on: 26 Apr 2019
 *      Author: one
 */
#include <stdio.h>
#include <unistd.h>
#include "check.h"

int main(int argc, char **argv)
{
	int res;
	int modifier;
	Sudoku s;

	if (argc < 2)
	{
		printf("Invalid argv.\n");
		return 0;
	}

	sscanf(argv[1], "%d", &modifier);
	read(STDIN_FILENO, &s, sizeof(s));

	switch (modifier) //Choose which function to run based on which child is active
	{
	case 0:
		res = checkRows(&s);
		break;

	case 1:
		res = checkCols(&s);
		break;

	case 2:
		res = checkBoxes(&s);
		break;

	default:
		return 0; //ERROR
	}
	write(STDOUT_FILENO, &res, sizeof(int));
	close(STDOUT_FILENO);
	close(STDIN_FILENO);
	return 1; 	//add return value through pipe.

}

int checkRows(Sudoku *s)
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
				return -1; //faulty information
		}

		for (j = 0; j < 9; ++j)
			if (check[j] != 1)
				return 10; //incorrect information

		for (j = 0; j < 9; ++j)
			check[j] = 0;
	}
	return 11;  //valid info
}

int checkCols(Sudoku *s)
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
				return -2; //invalid character
		}

		for (j = 0; j < COLS; ++j)
		{
			if (check[j] != 1)
				return 20; //incorrect Sudoku
		}
		for (j = 0; j < 9; ++j)
			check[j] = 0;
	}
	return 21; //valid result
}

int checkBoxes(Sudoku *s)
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
				if (s->fields[i * BOX + mult + ROWS * j + k] >= 1 && s->fields[i * BOX + mult + ROWS * j + k] <= 9)
					check[s->fields[i * BOX + mult + ROWS * j + k] - 1] = 1;
				else
					return -3; //invalid character
			}
		}
		for (j = 0; j < COLS; ++j)
		{
			if (check[j] != 1)
				return 30; //incorrect Sudoku
		}

		for (j = 0; j < 9; ++j)
			check[j] = 0;
	}
	return 31; //valid solution
}

