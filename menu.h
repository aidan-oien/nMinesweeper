// #include "shared.h"

//==================================================================================================//

enum option
{
    NEW_GAME,
    SAVED_GAME,
    LEADERBOARDS,
    OPTIONS,
    QUIT_GAME
};

//==================================================================================================//

int mainMenu( int selection );

GameSettings * difficultyMenu();

PlayerSelection menuInput( int length, PlayerSelection choice, char menuOptions[][MENU_STRING_LENGTH], WINDOW * win );

void renderMenu( int length, PlayerSelection choice, char options[][ MENU_STRING_LENGTH ], WINDOW * win, int color );

//==================================================================================================//
