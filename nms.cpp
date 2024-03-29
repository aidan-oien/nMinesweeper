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

char menu_options[][MENU_STRING_LENGTH] = {
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

//--------------------------------------------------------------------------------------------------// FUNCTIONS USED IN MAIN

int menu(int y, int x);
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

	while(on){
		switch(menu(0,4)){
			case 0:
				clear();
				switch(menu(9,14)){
					case 9:
						game(9,9,9,0);
						break;
					case 10:
						game(16,16,15,0);
						break;
					case 11:
						game(30,16,20,0);
						break;
					case 12:
						game(40,22,25,0);
						break;
					case 13:
						game(10,10,20,0);
						break;


				}
				break;
			
			case 1:
				on = 0;
			case 2:
				on = 0;
			case 3:
				switch(menu(15,17)){
					case 15:
						clear();
						break;
					case 16:
						clear();
						break;
				}
			case 4:
				on = 0;
		}
	}
	endwin();

	return 0;

}

//--------------------------------------------------------------------------------------------------// RENDER MENU

void renderMenu(int start, int end, int choice/*, char menu[][MENU_STRING_LENGTH]*/){
	
	int y = 2;

	for (int i = start; i <= end; i++){
		if(choice == i){
			attron(A_STANDOUT);
			mvprintw(y, 2, menu_options[i]);
			attroff(A_STANDOUT);
		} else {
			mvprintw(y, 2, menu_options[i]);
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
//--------------------------------------------------------------------------------------------------// RENDER BOARD

void renderBoard(struct cell **board, int y, int x, int yc, int xc, bool debug){
	for(int i = 0; i < y; i++){
		for(int j = 0; j < x; j++){
			if(xc==i && yc==j){
				attron(A_STANDOUT);
			}
			if(board[i][j].flagged == 1){ // ------------- Flag
				attron(COLOR_PAIR(10));
				mvprintw(i, j * 2, "F ");
				attroff(COLOR_PAIR(10));
			} else if(board[i][j].visible == 0 && !debug){ // -------------------- Covered
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

}

//--------------------------------------------------------------------------------------------------// FLAGGED ALL MINES

int flaggedAllMines(struct cell **board, bool &allFlagged, int y, int x, int flags, int mines){

	allFlagged = 1;

	for(int i = 0; i < y; i++){
		for(int j = 0; j < x; j++){
			if(board[i][j].flagged != board[i][j].mine || mines != flags){

				allFlagged = 0;

			}
		}
	}

	if(allFlagged == 1){
	return 1;
	}
	return -1;

}

//--------------------------------------------------------------------------------------------------// OPEN AREA

void openArea(struct cell **board, int i, int j){

	for(int k = i-1; k <= i+1; k++){
		for(int l = j-1; l <= j+1; l++){
			//Make sure index isn't negative
			if((k >= 0 && l >= 0)/*&&(k <= x && l <= y)*/){
				if(board[i][j].visible && !board[k][l].visible && board[i][j].number == 0 && board[k][l].mine == 0){
					board[k][l].visible = 1;
				}
			}
		}
	}

}

//--------------------------------------------------------------------------------------------------// MENU

int menu(int start, int end){

	int choice = start;

	clear();

	while(1){
		renderMenu(start,end,choice);
		if(menuSelect(start,end,choice)){
			return choice;
		}
	}

}

//--------------------------------------------------------------------------------------------------// GAME

void game(int y, int x, int ratio, bool savedGame){

	// Var setup
	int yc = 0, xc = 0, ym = 0, xm = 0, flags = 0, mines = 0, vic = -1;
	bool on = 0, allFlagged = 0, debug = 0, makingVisble = 0;


	// Create new board
	srand (time(NULL));

	struct cell **board = new struct cell*[y];

	for(int i = 0; i <= y; i++){
		board[i] = new struct cell[x];
	}


	// Populate board
	mines = (y * x) * (float)ratio/100;

	for(int i = 0; i < mines;){
		ym = rand() % y;
		xm = rand() % x;

		if(board[ym][xm].mine == 0){
			board[ym][xm].mine = 1;
			i++;
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
		renderBoard(board,y,x,yc,xc,debug);

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

			case 'o': // -------------------- Uncover Area and Hit Mine
				board[xc][yc].clicked = 1;
				board[xc][yc].visible = 1;
				makingVisble = 1;

				while(makingVisble){
					for(int i = 0; i < y; i++){ // Uncover Area
						for(int j = 0; j < x; j++){
							openArea(board,i,j);
						}
					}
					for(int i = y - 1; i > 0; i--){
						for(int j = y - 1; j > 0; j--){
							openArea(board,i,j);
						}
					}
					makingVisble = 0;

					for(int i = 0; i < y; i++){ // Uncover Area
						for(int j = 0; j < x; j++){
							for(int k = i-1; k <= i+1; k++){
								for(int l = j-1; l <= j+1; l++){
									//Make sure index isn't negative
									if((k >= 0 && l >= 0)/*&&(k <= x && l <= y)*/){
										if(board[i][j].visible && !board[k][l].visible && board[i][j].number == 0){
											makingVisble = 1;
										}
									}
								}
							}
						}
					}
				}

				if(board[xc][yc].clicked == board[xc][yc].mine){ // Hit mine
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
					erase();
					refresh();
				}
				break;
		}

		if(debug){
			mvprintw(0, x * 2, "gameEnd = %d ", vic);
			mvprintw(1, x * 2, "cNumber = %d ", board[xc][yc].number);
			mvprintw(2, x * 2, "isFlag? = %d ", board[xc][yc].flagged);
			mvprintw(3, x * 2, "isMine? = %d ", board[xc][yc].mine);
			mvprintw(4, x * 2, "xcValue = %d ", xc);
			mvprintw(5, x * 2, "ycValue = %d ", yc);
			mvprintw(6, x * 2, "ttlMine = %d ", mines);
			mvprintw(7, x * 2, "ttlFlag = %d ", flags);
			refresh();
		}
	}

	for(int i = 0; i < y; i++){ // Uncover Area
		for(int j = 0; j < x; j++){
			board[i][j].visible = 1;
		}
	}

	renderBoard(board,y,x,yc,xc,debug);
	
	//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

	//clear();

	if(vic == 1){
		printw("WIN");
	} else {
		printw("LOSE");
	}
	refresh();
	getch();

	// Delete board
	for(int i = 0; i < y; i++){
		delete[] board[i];
	}
	delete[] board;

}

//--------------------------------------------------------------------------------------------------//
