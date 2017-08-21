#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <bool.h>
using namespace std;

int **initBoard(); //initializes a board for play
void printBoard(int **board); //prints the current situation of the board
void printMenu(int **board); //prints the menu with options to proceed with game
void printOptions(); //prints the options
void runGame(); //runs the game
void initPlayers(int *player1, int *player2); //initializes players
int **nextTurn(int **board, int turn); //processes next turn
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
bool checkPlayer(char * str); //Checks for proper input for player/piece selection
bool checkRow(char * row); //Checks for acceptable row options
bool checkCol(char * col); //Checks for acceptable col options
bool checkSpot(int **board, int row, int col);//Checks if inputed spot has been played already 

bool checkSpot(int **board, int row, int col){
	if(board[row][col] != -1){
		return false;
	}
	else{
		return true;
	}
}
bool checkPlayer(char * player){
		if(strlen(player) == 1 && (player[0] == '0' || player[0] == '1')){
			return true;
		}
		else{
			return false;
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
	int i, j = 1;
	
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


void initPlayers(int *player1, int *player2){
	char * inpStr = NULL;
	printf("Player 1, would you like to play as X or as O. Enter 1 if X and 2 if O: ");
	inpStr = getString(stdin);
	while(!checkPlayer(inpStr)){
		printf("Invalid input, enter 1 or 2: ");
		inpStr = getString(stdin);
	}
	*player1 = atoi(inpStr);
	if(*player1 == 1){
		*player2 = 0;
	}
	else{
		*player2 = 1;
	}
}
void runGame(){
	int player1; //player 1
	int player2; //player 2
	int run = -1;
	int turn = 0;
	
	player1 = -1;
	player2 = -1;
	int **board = initBoard(); //initializes board
	initPlayers(&player1, &player2); //initializes players
	
	while(run == -1){ //runs game
		turn++;
		board = nextTurn(board, (turn % 2));
		run = checkBoard(board, turn);
	}
	if(run == 3){
		printf("Nobody wins, out of spaces!\n");
	}
	else if(run == 1){
		printf("Player 2 Wins!\n");
	}
	else if(run == 0){
		printf("Player 1 Wins!\n");
	}
	
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
int **nextTurn(int **board, int turn){
	char *row;
	char *col;
	int row2 = 0;
	int col2 = 0;
	int count = 0;
	printBoard(board);
	
	do{
		if(turn % 2 == 1){
			printf("Player One's Turn\n");
		}
		else{
			printf("Player Two's Turn\n");
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
	board[row2][col2] = turn;
	return board;
}
int checkBoard(int **board, int turns){
	int check = -1;
	if(turns == 9){
		return 3;
	}
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
	return check;
}
	
int checkDiagnolBotL(int **board){//check bottom left to top right
	if(board[2][0] == 1 && board[1][1] == 1 && board[0][2] == 1){ //check 3 X's in a row
		return 1;
	}
	if(board[2][0] == 0 && board[1][1] == 0 && board[0][2] == 0){ //check 3 circle's in a row
		return 0;
	}
	else{ //not in a row
		return -1;
	}
}
int checkDiagnolTopL(int **board){//check top left to bottom right
	if(board[0][0] == 1 && board[1][1] == 1 && board[2][2] == 1){//check X's
		return 1;
	}
	if(board[0][0] == 0 && board[1][1] == 0 && board[2][2] == 0){//check circles
		return 0;
	}
	else{//not in a row
		return -1;
	}
}
int checkTopRow(int **board){
	if(board[0][0] == 1 && board[0][1] == 1 && board[0][2] == 1){//check X's
		return 1;
	}
	else if(board[0][0] == 0 && board[0][1] == 0 && board[0][2] == 0){//check circles
		return 0;
	}
	else{
		return -1;
	}
}

int checkMidRow(int **board){
	if(board[1][0] == 1 && board[1][1] == 1 && board[1][2] == 1){
		return 1;
	}
    else if(board[1][0] == 0 && board[1][1] == 0 && board[1][2] == 0){
		return 0;
	}
	else{
		return -1;
	}
}

int checkBotRow(int **board){
	if(board[2][0] == 1 && board[2][1] == 1 && board[2][2] == 1){
		return 1;
	}
	else if(board[2][0] == 0 && board[2][1] == 0 && board[2][2] == 0){
		return 0;
	}	
	else{
		return -1;
	}
}

int checkLeftCol(int **board){	
	if(board[0][0] == 1 && board[1][0] == 1 && board[2][0] == 1){
		return 1;
	}
	else if(board[0][0] == 0 && board[1][0] == 0 && board[2][0] == 0){
		return 0;
	}	
	else{
		return -1;
	}
}

int checkMidCol(int **board){
	if(board[0][1] == 1 && board[1][1] == 1 && board[2][1] == 1){
		return 1;
	}
	else if(board[0][1] == 0 && board[1][1] == 0 && board[2][1] == 0){
		return 0;
	}	
	else{
		return -1;
	}
}
int checkRightCol(int **board){
	if(board[0][2] == 1 && board[1][2] == 1 && board[2][2] == 1){
		return 1;
	}
	else if(board[0][2] == 0 && board[1][2] == 0 && board[2][2] == 0){
		return 0;
	}	
	else{
		return -1;
	}
}


	
int **initBoard(){
	int **board = new int*[3]; //malloc for rows
	for(int k = 0; k < 3; k++){
		board[k] = new int[3]; //malloc columns
		for(int j = 0; j < 3; j++){
			board[k][j] = -1; //init value
		}
	}
	
	return board;
}
void printBoard(int **board){
	cout<<"\n   1 2 3\n";
	for(int i = 0; i < 3; i++){
		printf("%c ", 65 + i);
		for(int j = 0; j < 3; j++){
			if(board[i][j] == -1){ //empty space
				cout<< " -";
			}
			if(board[i][j] == 0){ //Circle occupies space
				cout<< " O";
			}
			if(board[i][j] == 1){ //X occupies space
				cout<< " X";
			}
		}
		cout<<"\n";
	}
}
int main(){
	
	int **board = initBoard();
	printBoard(board);
	
	runGame();
	return 1;
}