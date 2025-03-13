#include "shared.h"

//==================================================================================================//

typedef struct menuOption menuOption;

struct menuOption
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

menuOption menuInput( int length, menuOption choice, char menuOptions[][MENU_STRING_LENGTH], WINDOW * win );
void renderMenu( int length, menuOption selection, char options[][MENU_STRING_LENGTH], WINDOW * win );

//==================================================================================================//
