#include <ncurses.h>
#include <stdlib.h>
#include "shared.h"

//==================================================================================================//

// typedef struct menuOption menuOption;

// struct menuOption
// {
//     char isDoSomethingOption;
//     char isListOption;
//     char isNumberTypeBox;
//     char optionName[ MENU_STRING_LENGTH ];
//     char listOptionList[ 5 ][ MENU_STRING_LENGTH ];
// };

enum difficulty
{
    EASY,
    MEDIUM,
    HARD,
    EXPERT,
    CUSTOM
};

enum actionDescriptor
{
    NO_ACTION,
    SELECT,
    BACK = -1
};

//==================================================================================================//

int mainMenu( int selection );
GameSettings * difficultyMenu();

PlayerSelection menuInput( int length, PlayerSelection choice, char menuOptions[][ MENU_STRING_LENGTH ], WINDOW * win );
void renderMenu( int length, PlayerSelection choice, char options[][ MENU_STRING_LENGTH ], WINDOW * win, int color );

//==================================================================================================//

//
// Main Menu
// shows the main menu and only returns an option once something has been selected
//

int mainMenu( int selection )
{
    int x = 3, y = 2;
    int menuWidth = 17;
    int menuLength = 5;

    WINDOW * mainMenuWin = newwin( menuLength + 2, menuWidth, y, x );

    PlayerSelection choice;

    choice.ySelection = selection;
    choice.xSelection = 0;
    

    char mainMenuOptions[][ MENU_STRING_LENGTH ] =
    {
        "New Game     ",
        "Saved Game   ",
        "Leaderboards ",
        "Options      ",
        "Exit Game    " 
    };

    do
    {    
        choice = menuInput( menuLength, choice, mainMenuOptions, mainMenuWin );
    }
    while ( !choice.action );

    if ( choice.action == BACK ) return -1;

    delwin( mainMenuWin );

    return choice.ySelection;
}

//
// Difficulty Menu
// shows the difficulty menu and only returns a value once something has been selected
//

GameSettings * difficultyMenu()
{
    int x = 20, y = 2;
    int menuWidth = 11;
    int menuLength = 5;

    WINDOW * difficultyMenuWin = newwin( menuLength + 2, menuWidth, y, x );

    PlayerSelection choice;

    choice.xSelection = 0;
    choice.ySelection = 0;

    char mainMenuOptions[][ MENU_STRING_LENGTH ] =
    {
        "Easy   ",
        "Medium ",
        "Hard   ",
        "Expert ", 
        "Custom " 
    };

    do
    {
        choice = menuInput( menuLength, choice, mainMenuOptions, difficultyMenuWin );
    }
    while ( !choice.action );

    if ( choice.action == BACK ) return NULL;

    GameSettings * settings = malloc( sizeof( GameSettings ) );

    switch ( choice.ySelection )
    {
        case EASY:
            settings->xSize = 9;
            settings->ySize = 9;
            settings->mineRatio = 0.09;
            settings->useMineCount = 0;
            settings->useSuggestedMineRatio = 0;
            break;
            
        case MEDIUM:
            settings->xSize = 16;
            settings->ySize = 16;
            settings->mineRatio = 0.15;
            settings->useMineCount = 0;
            settings->useSuggestedMineRatio = 0;
            break;

        case HARD:
            settings->xSize = 30;
            settings->ySize = 16;
            settings->mineRatio = 0.20;
            settings->useMineCount = 0;
            settings->useSuggestedMineRatio = 0;
            break;

        case EXPERT:
            settings->xSize = 40;
            settings->ySize = 22;
            settings->mineRatio = 0.25;
            settings->useMineCount = 0;
            settings->useSuggestedMineRatio = 0;
            break;

        case CUSTOM:
            //settings = customGameMenu();
            return NULL;
            break;
    }

    delwin( difficultyMenuWin );

    return settings;   
}

//
// Menu Input
// takes input in from the user and refreshes the menu every time input is recieved
//

PlayerSelection menuInput( int length, PlayerSelection choice, char menuOptions[][ MENU_STRING_LENGTH ], WINDOW * win )
{
    renderMenu( length, choice, menuOptions, win, COLOR_MENU_ACTIVE );

    choice.action = 0;

    int ch = wgetch( win );

    switch( ch )
    {
        //case KEY_UP:
        case 'w':
            if ( choice.ySelection > 0) choice.ySelection--;
            else choice.ySelection = length - 1;
            break;
 
        //case KEY_DOWN:
        case 's':
            // if ( choice.ySelection < length - 1 ) choice.ySelection++;
            choice.ySelection++;
            choice.ySelection = choice.ySelection % length;
            break;

        case 'e':
            choice.action = SELECT;
            // this code greys out the menu once something is selected
            renderMenu( length, choice, menuOptions, win , COLOR_MENU_INACTIVE );
            break;

        case 27:
            choice.action = BACK;
            break;
    }

    return choice;
}

//
// Render Menu
// renders the menu after every input
//

void renderMenu( int length, PlayerSelection choice, char options[][ MENU_STRING_LENGTH ], WINDOW * win, int color )
{
    wattron( win, COLOR_PAIR( color ) );

	for ( int i = 0; i <= length; i++ )
	{
		if ( choice.ySelection == i )
        {
			wattron( win, A_STANDOUT );
			mvwprintw( win, i + 1, 2, options[ i ] );
			wattroff( win, A_STANDOUT );
		}
		else
		{
			mvwprintw( win, i + 1, 2, options[ i ] );
		}
	}

    box( win, 0, 0 );
    mvwprintw( win, choice.ySelection + 1, 0, ">" );

    wattroff( win, COLOR_PAIR( color ) );

	mvprintw( 0, 0, "nMinesweeper" );

    refresh();
    wrefresh( win );
}

//==================================================================================================//
