/*
	array of chars replaced with array of ints
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*	
	Function to check 3 x 3 grid around cell defined by co-ordinates given by 
	the arguments "row" and "column" for live neighbours
*/
int checkNeighbours(int **board, int row, int column){
	
	int neighbours = 0;

	int start_x = column - 1; /*	Moves start co-ordinate to top left of 3 x 3 grid	*/
	int start_y = row - 1;
	int x,y;
	for(y = 0; y < 3; y++){	/*	Nested Loops to cycle through 3 x 3 grid	*/
		for(x = 0; x < 3; x++){
				if(board[start_y + y][start_x + x] >= 0){
					neighbours++;				
				}
		}
	}	

	if(board[row][column] >= 0) neighbours--;		
	
	/*	Remove one neighbour if given cell is alive to avoid it being counted as a neighbour	*/

	return neighbours;
} 

/*
	Function to return an empty board array with dimensions given in arguments r and c
*/
int** getBoard(int r, int c){
	
	r+=2;
	c+=2;

	/*
		Adding 2 to r and c is done to add a border of empty cells around the board

		This avoids trying to access unallocated memory
	*/
	
	int i, j;
	int **board;
	
	board = (int **)malloc(r * sizeof(int *));
	for(i = 0; i < r; i++){
			board[i] = (int *)malloc(c * sizeof(int));	/*	Fill array with arrays	*/
	}
	
	for(i = 0; i < r; i++)
		for(j = 0; j < c; j++)
			board[i][j] = -1;	/*	Fills board with dead cells denoted by -1	*/
	
	return board;	
}

/*
	Function that takes board input and applies rules to it,
	returning a new board and deleting the old one
*/
int** getNextBoard(int **oldboard, int r, int c){
	
	int** board = getBoard(r, c);
	
	r+=2;
	c+=2;
	
	int x, y, neighbour;
	for(y = 1; y < r - 1; y++){	/*	Nested Loops to run through whole board	*/
		for(x = 1; x < c - 1; x++){
			neighbour = checkNeighbours(oldboard, y, x);
			
			if(oldboard[y][x] >= 0){
				
				if(neighbour < 2)	/*	Rule 1	*/
				{
					board[y][x] = -1;
				}
				
				
				if(neighbour == 2 || neighbour == 3)	/*	Rule 2	*/
				{
					board[y][x] = oldboard[y][x] + 1; /* adds one to cell if staying alive */
				}
				
				if(neighbour > 3)	/*	Rule 3	*/
				{
					board[y][x] = -1;
				}
				
			}else{
				
				if(neighbour == 3)	/*	Rule 4	*/
				{
					board[y][x] = 0;	/* 0 means new cell */
				}				
			}
		}
	}
	
	free(oldboard);
	return board;
}

void printBoard(int **board, int row, int column){
	
	int x;
	
	printf("*");	/*	Top Line	*/
	for(x = 0; x < column; x++){
		printf("-");
	}
	printf("*\n");
	

	int y;
	for(y = 1; y < row + 1; y++){	/*	Board	*/
		printf("|");
		for(x = 1; x < column + 1; x++){
		/* Loop starts at 1 to take into account border around board */

			if(board[y][x] >= 0){	/*if alive*/

				if(board[y][x] <= 9)
					printf("%i", board[y][x]);
				else putchar('x');

			}else printf(" ");	/*if dead*/	

		}
		printf("|\n");
	}

	printf("*");	/*	Bottom Line	*/
	for(x = 0; x < column; x++){
		printf("-");
	}
	printf("*\n\n");	
}

/*
	TESTING FUNCTION FOR DEBUGGING NOT USED IN PROGRAM
	
	prints out board with x's and spaces replaced by integers
	denoting number of neighbours each cell has
*/
void printNeighbourBoard(int **board, int row, int column){
	
	int x;
	
	printf("*");
	for(x = 0; x < column; x++){
		printf("-");
	}
	printf("*\n");	

	int y;
	for(y = 1; y < row + 1; y++){
		printf("|");
		for(x = 1; x < column + 1; x++){
			printf("%i", checkNeighbours(board, y, x));	
		}
		printf("|\n");
	}

	printf("*");
	for(x = 0; x < column; x++){
		printf("-");
	}
	printf("*\n");

}

/*
	Function called at runtime to open file and fill with cells
*/
int** populateBoard(int** board, char* filename, int row, int column){
		
	FILE* file = fopen(filename, "r");
	
	if(file == NULL){
		fprintf(stderr, "ERROR: File %s Failed To Open\n", filename);
		exit(EXIT_FAILURE);
	}
	
	int cells;	/*	Top line gives number of cells	*/
	fscanf(file, "%i", &cells);
	
	int i, r, c;
	for(i = 0; i < cells; i++){
		fscanf(file, "%i %i", &r, &c);
		
		if(r > row || c > column){
			fprintf(stderr, "ERROR: Co-ordinate found outside Board\n");
			exit(EXIT_FAILURE);
		}
		
		board[r + 1][c + 1] = 0;
		/* +1 in each case takes into account border around board */
	}

	fclose(file);	

	return board;
}

void main(int argc, char * argv[]) {
	
	int rounds = atoi(argv[4]);
	int column = atoi(argv[2]);
	int row = atoi(argv[3]);	
	
	if(rounds < 1){
		fprintf(stderr, "ERROR: Positive Rounds Required\n");
		exit(EXIT_FAILURE);
	}
	if(column < 1){
		fprintf(stderr, "ERROR: Positive Width Required\n");
		exit(EXIT_FAILURE);
	}
	if(row < 1){
		fprintf(stderr, "ERROR: Positive Height Required\n");
		exit(EXIT_FAILURE);
	}
	
	int ** board = getBoard(row, column);	/*	Create First Board	*/
	
	board = populateBoard(board, argv[1], row, column);	/*	Fill Board	*/
	
	printBoard(board, row, column);	/*	Print Board	*/
	
	int x;
	for(x = 0; x < rounds; x++){
		putchar('\n');
		board = getNextBoard(board, row, column);
		printBoard(board, row, column);
	}
	
	printf("Finished\n");
	
	free(board);
	
}
