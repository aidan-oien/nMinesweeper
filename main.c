//==================================================================================================//
//                                                                                             		//
//	       /'\_/`\  __                                                                           	//
//	  ___ /\      \/\_\    ___      __    ____  __  __  __     __     __   _____      __   _ __  	//
//	/' _ `\ \ \__\ \/\ \ /' _ `\  /'__`\ /',__\/\ \/\ \/\ \  /'__`\ /'__`\/\ '__`\  /'__`\/\`'__\	//
//	/\ \/\ \ \ \_/\ \ \ \/\ \/\ \/\  __//\__, `\ \ \_/ \_/ \/\  __//\  __/\ \ \L\ \/\  __/\ \ \/ 	//
//	\ \_\ \_\ \_\\ \_\ \_\ \_\ \_\ \____\/\____/\ \___,___/'\ \____\ \____\\ \ .__/\ \____\\ \_\ 	//
//	 \/_/\/_/\/_/ \/_/\/_/\/_/\/_/\/____/\/___/  \/__//__/   \/____/\/____/ \ \ \/  \/____/ \/_/ 	//
//                                                                         \ \_\               		//
//                                          	By: Aidan "Jack" Oien       \/_/               		//
//												(Thanks Weevil)										//
//																									//
//==================================================================================================//
//																									//
//	nMinesweeper is recreatation of the popular desktop game Minesweeper in C++ made for            //
//  the terminal by making use of the ncurses library for its UI and controls.						//
//																									//
//==================================================================================================//

// Complied on my deck with:
// gcc main.c menu.c game.c -lncurses -ltinfo -o nMinesweeper

#include <stdlib.h>
#include <ncurses.h>
#include "shared.h"
#include "menu.h"
#include "game.h"

#define COLOR_GRAY 9

//==================================================================================================//
// MAIN																								//
//==================================================================================================//

int main()
{
	// Init

	initscr();
	cbreak(); // removes the need for a character to be entered into the program, keystrokes will automatically be read
	noecho(); // makes sure key strokes entered into the program will not be echoed out
	keypad(stdscr, TRUE); // Allows the use of function keys
	curs_set( 0 ); 
	start_color();
	init_color( COLOR_BLACK, 0, 0, 0 );
	init_color( COLOR_GRAY, 400, 400, 400 );
	init_pair( 1, COLOR_BLUE, COLOR_BLACK ); // 1 and Uncovered
	init_pair( 2, COLOR_GREEN, COLOR_BLACK );
	init_pair( 3, COLOR_YELLOW, COLOR_BLACK );
	init_pair( 4, COLOR_MAGENTA, COLOR_BLACK );
	init_pair( 5, COLOR_RED, COLOR_BLACK );
	init_pair( 6, COLOR_CYAN, COLOR_BLACK );
	init_pair( 7, COLOR_WHITE, COLOR_BLACK );
	init_pair( 8, COLOR_GRAY, COLOR_BLACK );
	init_pair( 9, COLOR_BLACK, COLOR_RED ); // Flag
	init_pair( 10, COLOR_BLACK, COLOR_BLUE ); // Flag
	init_pair( 11, COLOR_BLUE, COLOR_BLACK ); // Covered

	int selection = 0;

    while ( 1 ) 
	{
		selection = mainMenu( selection );

		switch ( selection )
		{
			case NEW_GAME:
				gameSettings * settings = difficultyMenu();

				if ( settings == NULL ) break;
				
				// gameResults results = startGame( settings );
				// if ( results.win ) addLeaderboardEntry( results );
				
				free( settings );
				
				break;

			case LEADERBOARDS:
				//leaderboardsMenu();
				break;

			case OPTIONS:
				// int subselection = optionsMenu();
				// switch ( subselection )
				// {
				// 		case COLOR_THEME:
				// 			// do something
				//			break;
				// }
				break;

			case QUIT_GAME:
			default:
				//if ( quitGamePrompt )
				{
					endwin();
					return 0;
				}

		}
	}
}

//==================================================================================================//
// EOF																								//
//==================================================================================================//