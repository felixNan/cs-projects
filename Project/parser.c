#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"
#include "gameStructs.h"
#include "MainAux.h"
#include "fileFunc.h"
#include "solver.h"

#define inputSize 258
#define setValues 3
#define hintValues 2
#define maxValues 3
#define generateValues 2
#define invalidError printf("ERROR: invalid command\n")
#define erroneousError printf("Error: board contains erroneous values\n")

int checkIsInt(char *input) {/*Get input string and check if all chars are digits, return the int or -1 if not positive integer*/
	unsigned int i;
	int num = 0;
	for (i = 0; i < strlen(input); i++) {
		if (!((input[i] >= '0') && (input[i] <= '9')))
			return -1;
		num *= 10;
		num += input[i] - '0';
	}
	return num;
}

int checkInput(int *values, int col, int row, char *cmd) {
	/*TODO: Fix documentation*/
	/*Function checks if the input for set and hint has the correct amount of strings and is a valid input for current board
	 * userCol, userRow and userVal are the user inputs (if he entered any, else they valued at -2)
	 * col, row are current board size
	 * cmd is a string representing the command we need to check ,since hint and set have different requirements*/
	if (!strcmp(cmd, "set")) {
		if ((values[0] == -2) || (values[1] == -2) || (values[2] == -1))/*We didn't read three strings after "set"*/
			invalidError;
		else {
			if (((values[0] >= 0) && (values[0] < col * row))/*Make sure input is valid for board*/
			&& ((values[1] >= 0) && (values[1] < col * row))
					&& ((values[2] >= 0) && (values[2] <= col * row)))
				return 1;
			else
				printf("Error: value not in range 1-%d\n", row * col);
		}
	} else if (!strcmp(cmd, "hint")) {
		if ((values[0] == -2) || (values[1] == -2))/*We didn't read two strings after "hint"*/
			invalidError;
		else {
			if ((values[0] >= 0) && (values[0] < col * row) && (values[1] >= 0)
					&& (values[1] < col * row))/*Make sure input is valid for board*/
				return 1;
			else
				printf("Error: value not in range 1-%d\n", row * col);
		}
	} else if (!strcmp(cmd, "generate")) {
		if ((values[0] == -2) || (values[1] == -2))/*We didn't read two strings after "generate"*/
			invalidError;
		else {
			if ((values[0] >= 0) && (values[0] < col * row * col * row)
					&& (values[1] >= 0) && (values[1] < col * row * col * row))/*Make sure input is valid for board*/
				return 1;
			else
				printf("Error: value not in range 0-%d\n",
						col * row * col * row);
		}
	}
	return 0;
}

void readInput(gameState *metaBoard) {
	const char delim[5] = " \t\r\n";
	char input[inputSize] = { 0 };
	char *token;/*Separates input to tokens*/
	int values[maxValues], i;/*For the sake of readability we have excess variables*/
	while (!feof(stdin)) {
		for (i = 0; i < maxValues; i++) {
			values[i] = -2;
		}
		fflush(stdin);
		printf("Enter your command:\n");
		if (fgets(input, inputSize, stdin) != NULL) {
			if (input[256] != 0) {/*Skip current line, too much characters in input*/
				invalidError;
				while (input[256] != 0) {
					input[256] = 0;
					if (fgets(input, inputSize, stdin) == NULL) {
						printf("Error: fgets has failed\n");
						exit(0);
					}
				}
			} else {
				token = strtok(input, delim);
				if (token != NULL) {
					if (!strcmp(token, "solve")) {
						token = strtok(NULL, delim);
						if (token) {
							sendToFill(metaBoard, token, Solve);
						} else
							invalidError;
					} else if (!strcmp(token, "edit")) {
						token = strtok(NULL, delim);
						sendToFill(metaBoard, token, Edit);
					} else if (!(strcmp(token, "mark_errors"))
							&& (metaBoard->mode == Solve)) {
						token = strtok(NULL, delim);
						if (token) {
							if ((*token - '0' == 0) || (*token - '0' == 1))
								metaBoard->markError = *token - '0';
							else
								printf("Error: the value should be 0 or 1\n");
						} else
							invalidError;
					} else if (!(strcmp(token, "print_board"))
							&& (metaBoard->mode != Init)) {
						printBoard(metaBoard);
					} else if ((!(strcmp(token, "set")))
							&& (metaBoard->mode != Init)
							&& (metaBoard->filledCells
									!= metaBoard->cols * metaBoard->cols
											* metaBoard->rows * metaBoard->rows)) {
						for (i = 0; i < setValues; i++) {/*Reading at least three strings after "set", else invalid input*/
							token = strtok(NULL, delim);
							if (token)
								values[i] = checkIsInt(token) - 1;
						}
						values[2]++;/*Fix value of what the user wants to enter to the cell, unneeded decrement*/
						if (checkInput(values, metaBoard->cols, metaBoard->rows,
								"set"))
							setBoard(values[0], values[1], values[2], metaBoard,
									1);
					} else if (!(strcmp(token, "validate"))
							&& (metaBoard->mode != Init)) {
						if (isErroneous(metaBoard))
							erroneousError;
						else {
							i = validate(metaBoard);
							if (i == 1)
								printf(
										"Validation passed: board is solvable\n");
							else if (!i)
								printf(
										"Validation failed: board is unsolvable\n");
						}
					} else if (!strcmp(token, "generate")
							&& (metaBoard->mode == Edit)) {
						for (i = 0; i < generateValues; i++) {/*Reading at least two strings after "generate", else invalid input*/
							token = strtok(NULL, delim);
							if (token)
								values[i] = checkIsInt(token) - 1;
						}
						if (checkInput(values, metaBoard->cols, metaBoard->rows,
								"generate")) {
							if (metaBoard->filledCells != 0)
								printf("Error: board is not empty\n");
							else
								generateBoard(values[0], values[1], metaBoard);/*TODO: Replace with GENERATE AND MAKE ThE FUNCTION*/
						}

					} else if (!strcmp(token, "undo")
							&& (metaBoard->mode != Init)) {
						undo(metaBoard);
					} else if (!strcmp(token, "redo")
							&& (metaBoard->mode != Init)) {
						redo(metaBoard);
					} else if (!strcmp(token, "save")
							&& (metaBoard->mode != Init)) {
						token = strtok(NULL, delim);
						if (token) {
							saveFile(metaBoard, token);
						} else
							invalidError;
					} else if ((strcmp(token, "hint") == 0)
							&& (metaBoard->mode == Solve)) {
						for (i = 0; i < hintValues; i++) {/*Reading at least two strings after "hint", else invalid input*/
							token = strtok(NULL, delim);
							if (token)
								values[i] = checkIsInt(token) - 1;
						}
						if (checkInput(values, metaBoard->cols, metaBoard->rows,
								"hint")) {
							if (isErroneous(metaBoard))
								erroneousError;
							else
								hintBoard(values[0], values[1], metaBoard);/*TODO: Replace with ILP version*/
						}
					} else if (!strcmp(token, "num_solutions")
							&& (metaBoard->mode != Init)) {
						if (isErroneous(metaBoard))
							erroneousError;
						else
							numOfSol(metaBoard->gameBoard);/*MAKE SURE WE DON'T RUIN THE BOARD LIKE THIS!!! ORIGINALLY IT'S TEMPBOARD!!!*/
					} else if (!strcmp(token, "autofill")
							&& (metaBoard->mode == Solve)) {
						if (isErroneous(metaBoard))
							erroneousError;
						else {
							autoFill(metaBoard);
						}
					} else if (!strcmp(token, "reset")
							&& (metaBoard->mode != Init)) {
						resetGame(metaBoard);
					} else if (!strcmp(token, "exit")) {
						exitGame(metaBoard);
					} else {
						invalidError;
					}
				}
			}

		} else if (feof(stdin)) {
			exitGame(metaBoard);
		} else {
			printf("Error: fgets has failed\n");
			exit(0);
		}
	}
	exitGame(metaBoard);
}
