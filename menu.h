// #include "shared.h"

//==================================================================================================//

typedef struct menuSelection menuSelection;

struct menuSelection
{
	int confirm;
	int selection;
};

enum option
{
    NEW_GAME,
    LEADERBOARDS,
    OPTIONS,
    QUIT_GAME
};

//==================================================================================================//

int mainMenu( int selection );
gameSettings * difficultyMenu();

menuSelection menuInput( int length, menuSelection choice, char menuOptions[][MENU_STRING_LENGTH], WINDOW * win );
void renderMenu( int length, menuSelection selection, char options[][MENU_STRING_LENGTH], WINDOW * win );

//==================================================================================================//
