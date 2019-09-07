
#ifndef PIPES_H_
#define PIPES_H_

#define COLS 9
#define ROWS 9
#define BOX 3
char* file = "check";

typedef struct {
	int fields[COLS*ROWS];
	int status[3];
}Sudoku;

int checkCol(int i, Sudoku* s);
int checkRow(int i, Sudoku* s);
int checkBox(int i, Sudoku* s);

#endif /* SUDOKO_H_ */

