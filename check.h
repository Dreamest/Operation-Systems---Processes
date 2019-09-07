/*
 * check.h
 *
 *  Created on: 29 Apr 2019
 *      Author: one
 */

#ifndef CHECK_H_
#define CHECK_H_


#define COLS 9
#define ROWS 9
#define BOX 3

typedef struct {
	int fields[COLS*ROWS];
	int status[3];
}Sudoku;

int checkRows(Sudoku *s);
int checkCols(Sudoku *s);
int checkBoxes(Sudoku *s);


#endif /* CHECK_H_ */

