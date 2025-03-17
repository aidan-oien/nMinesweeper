// #include "shared.h"

//==================================================================================================//

typedef struct Tile Tile;

struct Tile
{
    int clicked;
	int flagged;
	int mine;
	int visible;
	int number;
};

//==================================================================================================//

GameResults * startGame( GameSettings * boardSettings );

Tile ** createBoard( GameSettings * boardSettings );

void deleteBoard( Tile ** board, GameSettings * boardSettings );

PlayerSelection gameInput( WINDOW * win, PlayerSelection choice, GameSettings * bounds );

void renderBoard( WINDOW * win, Tile ** board, GameSettings * boardSettings, PlayerSelection selection );

int calculateMineCount( GameSettings * boardSettings );

Tile ** plotMines( Tile ** board, GameSettings * boardSettings );

int min( int a, int b );

Tile ** plotNumbers( Tile ** board, GameSettings * boardSettings );

//==================================================================================================//
