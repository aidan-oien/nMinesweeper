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

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "shared.h"
#include "menu.h"
#include "game.h"

#define COLOR_GRAY 9



//==================================================================================================//
// MAIN																								//
//==================================================================================================//

int main()
{
	// init
	initscr();
	
	cbreak(); // removes the need for a character to be entered into the program, keystrokes will automatically be read
	noecho(); // makes sure key strokes entered into the program will not be echoed out
	keypad(stdscr, TRUE); // Allows the use of function keys
	
	curs_set( 0 ); 
	
	start_color();

	init_color( COLOR_BLACK,	0,		0,		0 );
	init_color( COLOR_GRAY,		200,	200,	400 );
	
	init_pair( 1,					COLOR_BLUE,		COLOR_GRAY );
	init_pair( 2,					COLOR_GREEN,	COLOR_GRAY );
	init_pair( 3,					COLOR_YELLOW,	COLOR_GRAY );
	init_pair( 4,					COLOR_MAGENTA,	COLOR_GRAY );
	init_pair( 5,					COLOR_RED,		COLOR_GRAY );
	init_pair( 6,					COLOR_CYAN,		COLOR_GRAY );
	init_pair( 7,					COLOR_WHITE,	COLOR_GRAY );
	init_pair( 8,					COLOR_BLACK,	COLOR_GRAY );
	init_pair( COLOR_MINE,			COLOR_WHITE,	COLOR_RED );
	init_pair( COLOR_FLAG,			COLOR_WHITE,	COLOR_BLUE );
	init_pair( COLOR_COVERED,		COLOR_WHITE,	COLOR_BLACK );
	init_pair( COLOR_UNCOVERED,		COLOR_WHITE,	COLOR_GRAY );
	init_pair( COLOR_MENU_ACTIVE,	COLOR_WHITE,	COLOR_BLACK );
	init_pair( COLOR_MENU_INACTIVE,	COLOR_GRAY,		COLOR_BLACK );


	// main
	int selection = 0;

    while ( 1 ) 
	{
		clear();
		refresh();
		selection = mainMenu( selection );

		switch ( selection )
		{
			case NEW_GAME:
				GameSettings * settings = difficultyMenu();
				if ( settings == NULL ) break;
				
				srand( time( NULL ) );

				GameResults * results = startGame( settings );
				// if ( results != NULL ) addLeaderboardEntry( results );

				free( settings );
				
				break;

			case SAVED_GAME:

				//loadGame();
				
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