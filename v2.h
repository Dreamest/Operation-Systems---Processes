/*
 * mmap.h
 *
 *  Created on: 4 May 2019
 *      Author: one
 */

#ifndef MMAP_H_
#define MMAP_H_

#define COLS 9
#define ROWS 9
#define BOX 3

typedef struct {
	int fields[COLS*ROWS];
	int status[3];
}Sudoku;

void decisionMaker(Sudoku *s, int i);
void checkRows(Sudoku *s);
void checkCols(Sudoku *s);
void checkBoxes(Sudoku *s);



#endif /* MMAP_H_ */
