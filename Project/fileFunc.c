#include <stdio.h>
#include <stdlib.h>
#include "gameStructs.h"
#include "linkedListFunc.h"
#include "solver.h"
#include "game.h"

void fillBoard(gameState *metaBoard, FILE* ifp) {
	int input, i, j, filled = 0;
	char *c, string[4] = { 0 };/*USING THE 5*5 LIMITATION, MAYBE USE CALLOC WITH log_10 instead*/
	freeBoard(metaBoard->gameBoard);
	fscanf(ifp, "%d", &input);
	metaBoard->gameBoard->cols = metaBoard->cols = input;
	filled += input;
	fscanf(ifp, "%d", &input);
	metaBoard->gameBoard->rows = metaBoard->rows = input;
	filled *= input;
	removeAllNext(metaBoard->moves->firstNode->next);/*Clear Undo/Redo list*/
	initalizeBoard(metaBoard->gameBoard);
	for (i = 0; i < metaBoard->rows * metaBoard->cols; i++) {
		for (j = 0; j < metaBoard->cols * metaBoard->rows; j++) {
			fscanf(ifp, "%s", string);
			c = string;
			if (*c == '0') {
				filled--;
			} else {
				while (c != '\0') {
					if (*c == '.') {
						metaBoard->gameBoard->board[i][j].fixed = 1;
					} else {
						metaBoard->gameBoard->board[i][j].value *= 10
								+ (*c - '0');
					}
					c++;
				}
			}
		}
	}
}

void saveFile(gameState *metaBoard, char *fileName) {
	FILE *ifp;
	int i, j, indexCounter[2] = { 0 };/*USED ONLY TEMPORARY NTIL WE DO VALIDATE ILP*/
	if (metaBoard->mode == Edit) {
		if (isErroneous(metaBoard)) {
			printf("Error: board contains erroneous values\n");
			return;
		} else {
			solver(metaBoard->gameBoard, indexCounter);
			if (!indexCounter[1]) {
				printf("Error: board validation failed\n");
				return;
			}
		}
	}
	ifp = fopen(fileName, "w");
	if (!ifp) {
		printf("Error: File cannot be created or modified\n");
		return;
	}
	fprintf(ifp, "%d ", metaBoard->gameBoard->cols);
	fprintf(ifp, "%d\n", metaBoard->gameBoard->rows);
	for (i = 0; i < metaBoard->cols; i++) {
		for (j = 0; j < metaBoard->rows; j++) {
			fprintf(ifp, "%d", metaBoard->gameBoard->board[i][j].value);
			if ((metaBoard->mode == Edit)
					|| (metaBoard->gameBoard->board[i][j].fixed))
				fprintf(ifp, "%c", '.');
			fprintf(ifp, " ");
		}
		fprintf(ifp, "\n");
	}
}
