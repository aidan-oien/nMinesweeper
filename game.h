// #include "shared.h"

//==================================================================================================//

typedef struct cell cell;

struct cell
{
    int clicked;
	int flagged;
	int mine;
	int visible;
	int number;
};

//==================================================================================================//

gameResults * startGame( gameSettings * boardSettings );
cell ** createBoard( gameSettings * boardSettings );
void deleteBoard( cell ** board, gameSettings * boardSettings );
void renderBoard( WINDOW * win, cell ** board, gameSettings * boardSettings );
int calculateMineCount( gameSettings * boardSettings );
int min( int a, int b );

//==================================================================================================//
