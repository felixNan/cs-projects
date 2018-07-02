#ifndef GAMESTRUCTS_H_
#define GAMESTRUCTS_H_

typedef struct cell{
	int value;
	int fixed;
	int error;
} cell;

typedef struct board{
	cell **board;
	int cols;
	int rows;
} board;

typedef enum gameMode{
	Init, Solve, Edit
}gameMode;

typedef struct node{
	int *change;
	struct node *next;
	struct node *prev;
}node;

typedef struct linkedList{
	node *firstNode;
	node *currentMove;
}linkedList;

typedef struct gameState{
	board *gameBoard;
	board *solution;/*NEED TO ERASE, OBSOLETE!*/
	gameMode mode;
	int markError;
	linkedList *moves;
	int filledCells;
	int cols;
	int rows;
} gameState;

#endif /* GAMESTRUCTS_H_ */