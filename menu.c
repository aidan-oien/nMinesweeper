#include <ncurses.h>
#include <stdlib.h>
#include "shared.h"

//==================================================================================================//

typedef struct menuSelection menuSelection;

struct menuSelection
{
	int confirm;
	int selection;
};

struct menuOption
{
    char isDoSomethingOption;
    char isListOption;
    char isNumberTypeBox;
    char optionName[ MENU_STRING_LENGTH ];
    char listOptionList[ 5 ][ MENU_STRING_LENGTH ];
};

enum difficulty
{
    EASY,
    MEDIUM,
    HARD,
    EXPERT,
    CUSTOM
};

//==================================================================================================//

int mainMenu( int selection );
gameSettings * difficultyMenu();
menuSelection menuInput( int length, menuSelection choice, char menuOptions[][MENU_STRING_LENGTH], WINDOW * win );
void renderMenu( int length, menuSelection choice, char options[][MENU_STRING_LENGTH], WINDOW * win );

//==================================================================================================//

int mainMenu( int selection )
{
    int x = 3, y = 2;
    int menuWidth = 17;
    int menuLength = 4;

    WINDOW * mainMenuWin = newwin( menuLength + 2, menuWidth, y, x );

    menuSelection choice;
    choice.selection = selection;
    choice.confirm = 0;

    char mainMenuOptions[][ MENU_STRING_LENGTH ] =
    {
        "New Game     ",
        "Leaderboards ",
        "Options      ",
        "Exit Game    " 
    };

    while ( !choice.confirm )
    {    
        choice = menuInput( menuLength, choice, mainMenuOptions, mainMenuWin );

        if ( choice.confirm ) break;
    }

    wattron( mainMenuWin, COLOR_PAIR( 8 ) );
    renderMenu( menuLength, choice, mainMenuOptions, mainMenuWin );
    wattroff( mainMenuWin, COLOR_PAIR( 8 ) );

    delwin( mainMenuWin );
    return choice.selection;
}

gameSettings * difficultyMenu()
{
    int x = 20, y = 2;
    int menuWidth = 11;
    int menuLength = 6;

    WINDOW * difficultyMenuWin = newwin( menuLength + 2, menuWidth, y, x );

    menuSelection choice;
    choice.selection = 0;
    choice.confirm = 0;

    char mainMenuOptions[][ MENU_STRING_LENGTH ] =
    {
        "EASY   ",
        "MEDIUM ",
        "HARD   ",
        "EXPERT \n", 
        "CUSTOM " 
    };

    while ( !choice.confirm )
    {
        choice = menuInput( menuLength, choice, mainMenuOptions, difficultyMenuWin );

        if ( choice.confirm ) break;
    }

    if ( choice.selection = -1 ) return NULL;

    gameSettings * settings = malloc( sizeof( gameSettings ) );

    switch ( choice.selection )
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

    wclear( difficultyMenuWin );
    wrefresh( difficultyMenuWin );
    delwin( difficultyMenuWin );

    return settings;   
}

menuSelection menuInput( int length, menuSelection choice, char menuOptions[][MENU_STRING_LENGTH], WINDOW * win )
{
    renderMenu( length, choice, menuOptions, win );

    int ch = wgetch( win );

    switch( ch )
    {
        case KEY_UP:
        case 'w':
            if (choice.selection > 0) choice.selection--;
            break;
 
        case KEY_DOWN:
        case 's':
            if ( choice.selection < length - 1 ) choice.selection++;
            break;

        case 'e':
            choice.confirm = 1;
            break;

        case 27:
            choice.selection = -1;
            choice.confirm = 1;
            break;
    }

    return choice;
}

void renderMenu( int length, menuSelection choice, char options[][MENU_STRING_LENGTH], WINDOW * win )
{
	int y = 1;

	for ( int i = 0; i <= length; i++ )
	{
		if ( choice.selection == i )
        {
			wattron( win, A_STANDOUT );
			mvwprintw( win, y, 2, options[ i ] );
			wattroff( win, A_STANDOUT );
		}
		else
		{
			mvwprintw( win, y, 2, options[ i ] );
		}

		y++;
	}

	mvprintw( 0, 0, "nMinesweeper" );

    box( win, 0, 0 );

    //mvwprintw( win, choice.selection + 1, 0, ">" );

    refresh();
    wrefresh( win );
}

//==================================================================================================//
