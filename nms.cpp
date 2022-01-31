//--------------------------------------------------------------------------------------------------//
//                                                                                             		//
//	       /'\_/`\  __                                                                           	//
//	  ___ /\      \/\_\    ___      __    ____  __  __  __     __     __   _____      __   _ __  	//
//	/' _ `\ \ \__\ \/\ \ /' _ `\  /'__`\ /',__\/\ \/\ \/\ \  /'__`\ /'__`\/\ '__`\  /'__`\/\`'__\	//
//	/\ \/\ \ \ \_/\ \ \ \/\ \/\ \/\  __//\__, `\ \ \_/ \_/ \/\  __//\  __/\ \ \L\ \/\  __/\ \ \/ 	//
//	\ \_\ \_\ \_\\ \_\ \_\ \_\ \_\ \____\/\____/\ \___x___/'\ \____\ \____\\ \ ,__/\ \____\\ \_\ 	//
//	 \/_/\/_/\/_/ \/_/\/_/\/_/\/_/\/____/\/___/  \/__//__/   \/____/\/____/ \ \ \/  \/____/ \/_/ 	//
//                                                                         \ \_\               		//
//                                          	By A. "Jack" O.             \/_/               		//
//												(Thanks Weevil)										//
//--------------------------------------------------------------------------------------------------//

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define MENU_STRING_LENGTH 14

struct cell {

	bool clicked = 0;
	bool flagged = 0;
	bool mine	 = 0;
	bool visible = 0;
	int  number	 = 0;

};

//--------------------------------------------------------------------------------------------------// FUNCTIONS USED IN MAIN

int menu();
void game(int y, int x, int ratio, bool savedGame);

//--------------------------------------------------------------------------------------------------// MAIN

int main(){

	// Var Setup

	bool on = 1;

	// Init

	initscr();
	cbreak();
	noecho();
	curs_set(0);
	start_color();
	init_color(COLOR_BLACK, 700, 700, 700);
	init_pair(1, COLOR_BLUE, COLOR_WHITE); // 1 and Uncovered
	init_pair(2, COLOR_GREEN, COLOR_WHITE);
	init_pair(3, COLOR_YELLOW, COLOR_WHITE);
	init_pair(4, COLOR_MAGENTA, COLOR_WHITE);
	init_pair(5, COLOR_RED, COLOR_WHITE);
	init_pair(6, COLOR_CYAN, COLOR_WHITE);
	init_pair(7, COLOR_BLACK, COLOR_WHITE);
	init_pair(8, COLOR_WHITE, COLOR_BLACK);
	init_pair(9, COLOR_BLACK, COLOR_RED); // Flag
	init_pair(10, COLOR_BLACK, COLOR_BLUE); // Flag
	init_pair(11, COLOR_BLUE, COLOR_BLACK); // Covered
	

	// Loop

	game(10,10,20,0); // Hard 30,16,20,0
	getch();
	

	/*
	while(on){

		switch(menu()) {

			case 0:

				clear();
				printw("Hi");
				getch();
				game(10,10,20,0);
				break;
			
			case 4:

				on = 0;

		}

	}
	*/

	endwin();

	return 0;

}

//--------------------------------------------------------------------------------------------------// RENDER MENU

void renderMenu(int start, int end, int choice, char menu[][MENU_STRING_LENGTH]){
	
	int y = 2;

	for (int i = start; i <= end; i++){
		if(choice == i){
			attron(A_STANDOUT);
			mvprintw(y, 2, menu[i]);
			attroff(A_STANDOUT);
		} else {
			mvprintw(y, 2, menu[i]);
		}
		y++;
	}

	refresh();
}

//--------------------------------------------------------------------------------------------------// MENU SELECT

bool menuSelect(int start, int end, int& choice){

	char c = getch();

	switch(c){
		case 'w':
			if(choice > start){
				(choice)--;
			}
			return 0;
		case 's':
			if(choice < end){
				(choice)++;
			}
			return 0;
		case 'e':
			return 1;
	}
	return 0;
}

//--------------------------------------------------------------------------------------------------// FLAGGED ALL MINES

int flaggedAllMines(struct cell **board, bool &allFlagged, int y, int x, int flags, int mines){

	allFlagged = 1;

	for(int i = 0; i < y; i++){
		for(int j = 0; j < x; j++){
			if(board[i][j].flagged != board[i][j].mine || mines != flags){

				allFlagged = 0;

			}

			/*
			mvprintw(0,60,"iValue  = %d  ",i);
			mvprintw(1,60,"jValue  = %d  ",j);
			mvprintw(2,60,"isMine? = %d  ",board[i][j].mine);
			mvprintw(3,60,"isFlag? = %d  ",board[i][j].flagged);
			mvprintw(4,60,"mines   = %d  ",mines);
			mvprintw(5,60,"flags   = %d  ",flags);
			mvprintw(6,60,"allflgd = %d  ",allFlagged);
			refresh();
			getch();
			*/

		}
	}

	if(allFlagged == 1){
	return 1;
	}
	return -1;
}

//--------------------------------------------------------------------------------------------------// MENU

int menu(){

	char menu[][MENU_STRING_LENGTH] = {
		"New Game     ", // 00-04 Main menu
		"Saved Game   ",
		"Leaderboards ",
		"Options      ",
		"Exit Game    ",
		"Continue     ", // 05-08 In-game menu
		"Save Game    ",
		"Options      ",
		"Quit Game    ",
		"Beginner     ", // 09-14 Difficulty menu
		"Intermediate ",
		"Hard         ",
		"Expert       ",
		"Custom       ",
		"< Back       ",
		"Keybinds     ", // 15-17 Options menu
		"Themes       ",
		"< Back       ",
	};

	int choice = 0;

	clear();

	while(1){
		renderMenu(0,4,choice,menu);
		if(menuSelect(0,4,choice)){
			return choice;
		}
	}
}

//--------------------------------------------------------------------------------------------------// GAME

void game(int x, int y, int ratio, bool savedGame){

	// Var setup
	int yc = 0, xc = 0, flags = 0, mines = 0, vic = -1;
	bool on = 0, allFlagged = 0, debug = 0;


	// Create new board
	srand (time(NULL));

	struct cell **board = new struct cell*[y];

	for(int i = 0; i <= y; i++){
		board[i] = new struct cell[x];
	}


	// Populate board
	for(int i = 0; i < y; i++){
		for(int j = 0; j < x; j++){
			if((((rand()%100)+ratio)/100)==1){
				board[i][j].mine = 1;
				mines++;
			}
		}
	}


	// Plotting numbers
	for(int i = 0; i < y; i++){
		for(int j = 0; j < x; j++){
			//Surround check
			for(int k = i-1; k <= i+1; k++){
				for(int l = j-1; l <= j+1; l++){
					//Make sure index isn't negative
					if((k >= 0 && l >= 0)/*&&(k <= x && l <= y)*/){
						if(board[k][l].mine){
							(board[i][j].number)++;
						}
					}
				}
			}
		}
	}


	// GAME LOOP /\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

	while(vic == -1){

		// RENDER RENDER RENDER RENDER RENDER RENDER RENDER RENDER RENDER RENDER RENDER RENDER RENDER REND
		for(int i = 0; i < y; i++){
			for(int j = 0; j < x; j++){
				if(xc==i && yc==j){
					attron(A_STANDOUT);
				}
				
				if(board[i][j].flagged == 1){ // ------------- Flag
					attron(COLOR_PAIR(10));
					mvprintw(i, j * 2, "F ");
					attroff(COLOR_PAIR(10));
				} else if(board[i][j].visible == 0){ // -------------------- Covered
					attron(COLOR_PAIR(11));
					mvprintw(i, j * 2, "  ");
					attroff(COLOR_PAIR(11));
				} else if(board[i][j].mine == 1){ // ---------------- Mine
					attron(COLOR_PAIR(9));
					mvprintw(i, j * 2, "M ");
					attroff(COLOR_PAIR(9));
				} else if(board[i][j].number != 0){ // -------------- Number
					attron(COLOR_PAIR(board[i][j].number));
					mvprintw(i, j * 2, "%d ", board[i][j].number);
					attroff(COLOR_PAIR(board[i][j].number));
				} else { // ----------------------------------------- Space
					attron(COLOR_PAIR(1));
					mvprintw(i, j * 2, "  ");
					attroff(COLOR_PAIR(1));
				}

				attroff(A_STANDOUT);
				refresh();
			}
		}

		// CONTROL CONTROL CONTROL CONTROL CONTROL CONTROL CONTROL CONTROL CONTROL CONTROL CONTROL CONTROL 
		switch(getch()){
			case 'w': // -------------------- Up
				if(xc > 0){
					xc--;
				}
				break;
			case 'a': // -------------------- Right
				if(yc > 0){
					yc--;
				}
				break;
			case 's': // -------------------- Left
				if(xc < y - 1){
					xc++;
				}
				break;
			case 'd': // -------------------- Down
				if(yc < x - 1){
					yc++;
				}
				break;
			case 'o': // -------------------- Uncover Area
				board[xc][yc].clicked = 1;
				board[xc][yc].visible = 1;
				if(board[xc][yc].clicked == board[xc][yc].mine){ //hit mine
					vic = 0;
				}
				break;
			case 'p': // -------------------- Flag
				if(board[xc][yc].visible == 0 && board[xc][yc].flagged == 0){
					board[xc][yc].flagged = 1;
					flags++;
					vic = flaggedAllMines(board,allFlagged,y,x,flags,mines);
				} else {
					board[xc][yc].flagged = 0;
					flags--;
					vic = flaggedAllMines(board,allFlagged,y,x,flags,mines);
				}
				break;
			case 'm': // -------------------- Debug menu
				if(!debug){
					debug = 1;
				} else {
					debug = 0;
					clear();
				}
				break;
		}

		if(debug){
			mvprintw(0,x+11,"gameEnd = %d ",vic);
			mvprintw(1,x+11,"cNumber = %d ",board[xc][yc].number);
			mvprintw(2,x+11,"isFlag? = %d ",board[xc][yc].flagged);
			mvprintw(3,x+11,"isMine? = %d ",board[xc][yc].mine);
			mvprintw(4,x+11,"xcValue = %d ",xc);
			mvprintw(5,x+11,"ycValue = %d ",yc);
			mvprintw(6,x+11,"ttlMine = %d ",mines);
			mvprintw(7,x+11,"ttlFlag = %d ",flags);
			refresh();
		}
	}

	//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

	clear();

	if(vic == 1){
		printw("WIN");
	} else {
		printw("LOSE");
	}
	refresh();

	// Delete board
	for(int i = 0; i < y; i++){
		delete[] board[i];
	}
	delete[] board;
}

//--------------------------------------------------------------------------------------------------//
