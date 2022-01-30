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

// cd /mnt/d/Users/Aidan/Documents/'My Stuff'/Code/nms

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

//--------------------------------------------------------------------------------------------------//

int menu();
void game(int y, int x, int ratio, bool savedGame);

//--------------------------------------------------------------------------------------------------//
// MAIN																								//
//--------------------------------------------------------------------------------------------------//

int main(){

	// Var Setup

	bool on = 1;

	// Init

	initscr();
	cbreak();
	noecho();
	curs_set(0);
	start_color();
	init_pair(2, COLOR_RED, COLOR_BLACK);

	// Loop

	game(30,16,20,0);

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

//--------------------------------------------------------------------------------------------------//
// RENDER FUNCTIONS																					//
//--------------------------------------------------------------------------------------------------//

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


//--------------------------------------------------------------------------------------------------//
// MECHANIC FUNCTIONS																				//
//--------------------------------------------------------------------------------------------------//

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

//--------------------------------------------------------------------------------------------------//

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

//--------------------------------------------------------------------------------------------------//

void game(int x, int y, int ratio, bool savedGame){

	int yc = 0, xc = 0, vic = -1;
	bool on = 0, allFlagged = 0;

	// Random seed
	srand (time(NULL));

	// Create new board
	struct cell **board = new struct cell*[y];

	for(int i = 0; i <= y; i++){

		board[i] = new struct cell[x];

	}

	// Populate board
	for(int i = 0; i < y; i++){

		for(int j = 0; j < x; j++){
			
			if((((rand()%100)+ratio)/100)==1){

				board[i][j].mine = 1;

			}

		}

	}

	board[4][4].mine = 1;

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

	// GAME LOOP

	while(vic == -1){

		// Control
		switch(getch()){

			case 'w':

				if(xc > 0){

					xc--;

				}
				
				break;

			case 'a':

				if(yc > 0){
				
					yc--;

				}

				break;

			case 's':

				if(xc < y - 1){

					xc++;

				}

				break;
		
			case 'd':

				if(yc < x - 1){

					yc++;

				}

				break;

			case 'e':

				board[xc][yc].clicked = 1;

				if(board[xc][yc].clicked == board[xc][yc].mine){ //hit mine

					vic = 0;

				}

				break;

			case 'f': 

				allFlagged = 1;

				board[xc][yc].flagged = 1;

				for(int i = 0; i < y; i++){

					for(int j = 0; j < x; j++){
			
						if(board[xc][yc].flagged == board[xc][yc].mine){

							allFlagged = 0;

						}

					}

				}

				if(allFlagged == 1){

					vic = 1;

				}

				break;

		}

		// Render
		for(int i = 0; i < y; i++){

			for(int j = 0; j < x; j++){
				
				
				if(xc==i && yc==j){
					
					attron(A_STANDOUT);

				}
				
				if(board[i][j].mine == 1){

					mvprintw(i, j * 2, "M ");

				} else if(board[i][j].number != 0){
					attron(COLOR_PAIR(2));
					mvprintw(i, j * 2, "%d ", board[i][j].number);
					attroff(COLOR_PAIR(2));
				} else {

					mvprintw(i, j * 2, "  ");

				}

				attroff(A_STANDOUT);
				refresh();
				//beanos
				
			}

		}

	}

	// Delete board
	for(int i = 0; i < y; i++){

		delete[] board[i];

	}

	delete[] board;

}
