#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <bool.h>
using namespace std;


//ENUMERATIONS
const int FREE_SPACE = -1, CONTINUE = -1; //EMPTY SPACE ON BOARD
const int X_VAL = 1; //SPACE OCCUPIED BY A PIECE 'X'
const int O_VAL = 0; //SPACE OCCUPIED BY A PIECE 'O'
const int PLAYER1 = X_VAL;
const int PLAYER2 = O_VAL;
const int SINGLEPLAYER = 1;
const int MULTIPLAYER = 2;
const int TIE = 3; //GAME ENDS IN A TIE

//FUNCTION DECLARATIONS
int **initBoard(); //initializes a board for play
void printBoard(int **board); //prints the current situation of the board
//void printMenu(int **board); //prints the menu with options to proceed with game
void printOptions(); //prints the options
void runGame(); //runs the game
int **nextTurn(int **board, int turn, int gameMode, int PLAYER1, int PLAYER2); //processes next turn
int checkBoard(int **board, int turns); //checks to see if the board has 3 pieces of the same in a row or out of spaces
int checkTopRow(int **board); //checks top row for 3 pieces in a row
int checkMidRow(int **board); //checks mid row for 3 pieces in a row
int checkBotRow(int **board); //checks bot row for 3 pieces in a row
int checkLeftCol(int **board); //checks left column for 3 pieces in a row
int checkMidCol(int **board); //checks mid column for 3 pieces in a row
int checkRightCol(int **board); //checks right column for 3 pieces in a row
int checkDiagnolTopL(int **board); //checks the diagnol from top left to bottom right of board for 3 pieces in a row
int checkDiagnolBotL(int **board); //checks the diagol form bottome left to top right for 3 pieces in a row
char * getString(FILE * stream); //dynamically obtains string from stream up to newline
char * capitalizeString(char * str); //capitalizes lower case letters in a given string
bool checkRow(char * row); //Checks for acceptable row options
bool checkCol(char * col); //Checks for acceptable col options
bool checkSpot(int **board, int row, int col);//Checks if inputed spot has been played already 
bool checkGameOption(char * gameOption); //CHECKS FOR VALID INPUT OF GAME MODE
int initGameMode(); //INITIALIZES GAME MODE - SINGLE OR MULTIPLAYER
//MINIMAX FUNC DECLARATIONS
int minimax(int ** board, int AI, int player, int turns);//DETERMINES SCORE OF NEXT MOVE
void AIMove(int ** board, int turns, int AI);//MAKES NEXT MOVE FOR COMPUTER

int main(){
	//CREATE AND INITIALIZE THE PLAYING BOARD
	int **board = initBoard();
	printBoard(board);
	
	//BEGIN THE GAME
	runGame();

	return 1;
}

//FUNCTION DEFINITIONS
using namespace std;
void runGame(){
	int run = CONTINUE;
	int turn = 0;
	int **board = initBoard(); //initializes board
	int gameMode = initGameMode();//DECIDES SINGLE/MULTI PLAYER
	printf("PLAYER1 is 'X' -> PLAYER2 is 'O'\n");
	while(run == CONTINUE){ //runs game
		turn++;
		board = nextTurn(board, turn, gameMode, PLAYER1, PLAYER2);
		run = checkBoard(board, turn);
	}
	if(run == TIE){
		printf("Nobody wins, it's a TIE!\n");
	}
	else if(run == O_VAL){
		printf("Player 2 Wins!\n");
		printBoard(board);
	}
	else if(run == X_VAL){
		printf("Player 1 Wins!\n");
		printBoard(board);
	}
	
}

void AImove(int ** board, int turns, int AI){
	int row = -1;
	int col = -1;
	int maxRow = -1;
	int maxCol = -1;
	int i, i2;
	int maxPoints = -1000000;
	int points = 0;	
	//int tempBoard[3][3] = 
	for(i = 0; i < 3; i++){
		for(i2 = 0; i2 < 3; i2++){
			if(board[i][i2] == FREE_SPACE){
				board[i][i2] = AI;
				points = minimax(board, AI, PLAYER1, turns);
				board[i][i2] = FREE_SPACE;
				if(points > maxPoints){
					maxRow = i;
					maxCol = i2;
					maxPoints = points;
				}
			}		
		}
	}
	board[maxRow][maxCol] = AI;
}

int minimax(int ** board, int AI, int player, int turns){
	int winner = checkBoard(board, turns);	
	if(winner == O_VAL){
		return 10;
	}
	if(winner == TIE){
		return 0;
	}
	if(winner == X_VAL){
		return -10;
	}

	else{
		if(player == AI){//FIND THE BEST MOVE -> PLAYER IS AI
			int best = -1000;
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++){
					if(board[i][j] == FREE_SPACE){
						board[i][j] = player;
						int temp = minimax(board, AI, PLAYER1, turns + 1);
						if(temp > best){
							best = temp;
						}
						board[i][j] = FREE_SPACE;					
					}
				}
			}
			return best;
		}
		else{ //FIND THE WORST MOVE -> PLAYER IS HUMAN
			int best = 1000;
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++){
					if(board[i][j] == FREE_SPACE){
						board[i][j] = player;
						int temp = minimax(board, AI, PLAYER2, turns + 1);
						if(temp < best){
							best = temp;
						}
						board[i][j] = FREE_SPACE;					
					}
				}
			}
			return best;
		}
	}
}

int ** nextTurn(int **board, int turn, int gameMode, int PLAYER1, int PLAYER2){
	char *row;
	char *col;
	int row2 = 0;
	int col2 = 0;
	int count = 0;
	if(gameMode == MULTIPLAYER){//MULTIPLAYER MODE
		do{
			printBoard(board);
			if(turn % 2 == 1){
				printf("PLAYER1's Turn\n");
			}
			else{
				printf("PLAYER2's Turn\n");
			}
			if(count != 0){
				printf("Invalid Input, Try again\n");
			}
			printf("What row - A, B, C - would you like to place your piece: ");
			row = getString(stdin);
			while(!checkRow(row)){
				printf("Invalid input, Enter 'A', 'B', or 'C': ");
				row = getString(stdin);
			}
	
			printf("What col - 1, 2, 3 - would you like to place your piece: ");
			col = getString(stdin);
			while(!checkCol(col)){
				printf("Invalid input, Enter '1', '2', or '3': ");
				col = getString(stdin);
			}
			count++;
			
			if(row[0] == 'A'){
				row2 = 0;
			}	
			else if(row[0] == 'B'){
				row2 = 1;
			}
			else if(row[0] == 'C'){
				row2 = 2;
			}
	
			col2 = atoi(col) - 1;
		}while(!checkSpot(board, row2, col2));
		board[row2][col2] = turn % 2;
	}
	
	else{//SINGLE PLAYER MODE
		if(turn % 2 == PLAYER1){//PLAYER 1's TURN
		printBoard(board);
		printf("PLAYER1's Turn\n");
			do{
				if(count != 0){
					printf("Invalid Input, Try again\n");
				}
				printf("What row - A, B, C - would you like to place your piece: ");
				row = getString(stdin);
				while(!checkRow(row)){
					printf("Invalid input, Enter 'A', 'B', or 'C': ");
					row = getString(stdin);
				}
	
				printf("What col - 1, 2, 3 - would you like to place your piece: ");
				col = getString(stdin);
				while(!checkCol(col)){
					printf("Invalid input, Enter '1', '2', or '3': ");
					col = getString(stdin);
				}
				count++;
			
				if(row[0] == 'A'){
					row2 = 0;
				}	
				else if(row[0] == 'B'){
					row2 = 1;
				}
				else if(row[0] == 'C'){
					row2 = 2;
				}
	
				col2 = atoi(col) - 1;
			}while(!checkSpot(board, row2, col2));
			board[row2][col2] = PLAYER1;
		}
		else{//AI's TURN
			AImove(board, turn, PLAYER2);
		}
	}
	return board;
}

void printBoard(int **board){
	//cout<<"\n   1 2 3\n";
	printf("\n   1 2 3\n");	
	for(int i = 0; i < 3; i++){
		printf("%c ", 65 + i);
		for(int j = 0; j < 3; j++){
			if(board[i][j] == -1){ //empty space
				//cout<< " -";
				printf(" -");			
			}
			if(board[i][j] == 0){ //Circle occupies space
				//cout<< " O";
				printf(" 0");			
			}
			if(board[i][j] == 1){ //X occupies space
				//cout<< " X";
				printf(" X");
			}
		}
		//cout<<"\n";
		printf("\n");
	}
}
int checkBoard(int **board, int turns){
	int check = -1;
	check = checkTopRow(board);
	if(check != -1){
		return check;
	}
	check = checkMidRow(board);
	if(check != -1){
		return check;
	}	
	check = checkBotRow(board);
	if(check != -1){
		return check;
	}
	check = checkLeftCol(board);
	if(check != -1){
		return check;
	}
	check = checkMidCol(board);
	if(check != -1){
		return check;
	}
	check = checkRightCol(board);
	if(check != -1){
		return check;
	}
	check = checkDiagnolBotL(board);
	if(check != -1){
		return check;
	}
	check = checkDiagnolTopL(board);
	if(check != -1){
		return check;
	}
	if(turns == 9){
		return 3;
	}
	return check;
}
	
int checkDiagnolBotL(int **board){//check bottom left to top right
	if(board[2][0] == X_VAL && board[1][1] == X_VAL && board[0][2] == X_VAL){ //check 3 X's in a row
		return X_VAL;
	}
	if(board[2][0] == O_VAL && board[1][1] == O_VAL && board[0][2] == O_VAL){ //check 3 circle's in a row
		return O_VAL;
	}
	else{ //not in a row
		return -1;
	}
}
int checkDiagnolTopL(int **board){//check top left to bottom right
	if(board[0][0] == X_VAL && board[1][1] == X_VAL && board[2][2] == X_VAL){//check X's
		return X_VAL;
	}
	if(board[0][0] == O_VAL && board[1][1] == O_VAL && board[2][2] == O_VAL){//check circles
		return O_VAL;
	}
	else{//not in a row
		return -1;
	}
}
int checkTopRow(int **board){
	if(board[0][0] == X_VAL && board[0][1] == X_VAL && board[0][2] == X_VAL){//check X's
		return X_VAL;
	}
	else if(board[0][0] == O_VAL && board[0][1] == O_VAL && board[0][2] == O_VAL){//check circles
		return O_VAL;
	}
	else{
		return -1;
	}
}

int checkMidRow(int **board){
	if(board[1][0] == X_VAL && board[1][1] == X_VAL && board[1][2] == X_VAL){
		return X_VAL;
	}
    	else if(board[1][0] == O_VAL && board[1][1] == O_VAL && board[1][2] == O_VAL){
		return O_VAL;
	}
	else{
		return -1;
	}
}

int checkBotRow(int **board){
	if(board[2][0] == X_VAL && board[2][1] == X_VAL && board[2][2] == X_VAL){
		return X_VAL;
	}
	else if(board[2][0] == O_VAL && board[2][1] == O_VAL && board[2][2] == O_VAL){
		return O_VAL;
	}	
	else{
		return -1;
	}
}

int checkLeftCol(int **board){	
	if(board[0][0] == X_VAL && board[1][0] == X_VAL && board[2][0] == X_VAL){
		return X_VAL;
	}
	else if(board[0][0] == O_VAL && board[1][0] == O_VAL && board[2][0] == O_VAL){
		return O_VAL;
	}	
	else{
		return -1;
	}
}

int checkMidCol(int **board){
	if(board[0][1] == X_VAL && board[1][1] == X_VAL && board[2][1] == X_VAL){
		return X_VAL;
	}
	else if(board[0][1] == O_VAL && board[1][1] == O_VAL && board[2][1] == O_VAL){
		return O_VAL;
	}	
	else{
		return -1;
	}
}
int checkRightCol(int **board){
	if(board[0][2] == X_VAL && board[1][2] == X_VAL && board[2][2] == X_VAL){
		return X_VAL;
	}
	else if(board[0][2] == O_VAL && board[1][2] == O_VAL && board[2][2] == O_VAL){
		return O_VAL;
	}	
	else{
		return -1;
	}
}
bool checkSpot(int **board, int row, int col){
	if(board[row][col] != -1){
		return false;
	}
	else{
		return true;
	}
}

char * capitalizeString(char * str){
	int iter = 0;
	while(str[iter] != '\0'){
		if(str[iter] > 96 && str[iter] < 123){
			str[iter] = str[iter] - 32;
		}
		iter++;
	}
	return str;
}
char * getString(FILE * stream){
	int i = 0, j = 1;
	
	char * str = (char*) malloc(sizeof(char));
	if(str == NULL){
		printf("INVALID MALLOC, str == NULL\n");
	}
	char c = getc(stream);
	
	while(c == ' ' || c == '\n'){
		c = getc(stream);
	}
	
	while(c != '\n'){
		str = (char*) realloc(str, j * sizeof(char) + 1);		
		if(str == NULL){
			printf("INVALID REALLOC, str == NULL\n");
		}
		str[i] = c;		
		i++;
		j++;
		c = getc(stream);
	}
	str[i] = '\0';
	str = capitalizeString(str);
	return str;
}

bool checkRow(char * row){
	if(strlen(row) == 1 && (row[0] == 'A' || row[0] == 'B' || row[0] == 'C')){
		return true;
	}
	else{
		return false;
	}
}
 
bool checkCol(char * col){
	if(strlen(col) == 1 && (col[0] == '1' || col[0] == '2' || col[0] == '3')){
		return true;
	}
	else{
		return false;
	}
}


bool checkGameOption(char * gameOption){
	//STRING SHOULD BE ONE CHARACTER LONG AND EITHER EQUAL TO 1 OR 2;
	return(strlen(gameOption) == 1 && (gameOption[0] == '1' || gameOption[0] == '2'));
}

int **initBoard(){
	//ALLOCATING MEMORY FOR ROWS
	int **board = (int **)malloc(sizeof(int) * 3);
	for(int k = 0; k < 3; k++){
		//ALLOCATING MEMORY FOR COLUMNS
		board[k] = (int *)malloc(sizeof(int) * 3);
		for(int j = 0; j < 3; j++){
			//INITIALIZING VALUE TO FREE SPACE
			board[k][j] = FREE_SPACE;
		}
	}
	return board;
}

int initGameMode(){
	char * gameMode; //gameMode will equal 1 for single player or 2 for two player
	
	//SELECT GAME OPTION
	printf("Player 1, Enter '1' for single player. Enter '2' for two player: ");
	gameMode = getString(stdin);
	while(!checkGameOption(gameMode)){
		printf("Invalid input, enter 1 for single player, 2 for two player: ");
		gameMode = getString(stdin);
	}
	int gameModeInt = atoi(gameMode);		
	return (gameModeInt);	

}
