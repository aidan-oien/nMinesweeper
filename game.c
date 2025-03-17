// TODO:
// make a queue for flood fill
// make it so you will always hit an open space when starting


#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include "shared.h"

//==================================================================================================//

typedef struct Tile Tile;
typedef struct TileQueue TileQueue;

struct Tile
{
    int clicked;
	int flagged;
	int mine;
	int visible;
	int number;
};

struct TileQueue
{
    PlayerSelection selection;
    TileQueue * next;
};

enum actionDescriptor
{
    NO_ACTION,
    UNCOVER,
    FLAG,
    BACK = -1
};

//==================================================================================================//

GameResults * startGame( GameSettings * boardSettings );
Tile ** createBoard( GameSettings * boardSettings );
void deleteBoard( Tile ** board, GameSettings * boardSettings );
int calculateMineCount( GameSettings * boardSettings );
Tile ** plotMines( Tile ** board, GameSettings * boardSettings );
Tile ** plotNumbers( Tile ** board, GameSettings * boardSettings );
PlayerSelection gameInput( WINDOW * win, PlayerSelection choice, GameSettings * bounds );
void renderBoard( WINDOW * win, Tile ** board, GameSettings * boardSettings, PlayerSelection selection );
int min( int a, int b );

//==================================================================================================//

//
// Start Game
// This is the main game loop
//

GameResults * startGame( GameSettings * boardSettings )
{

    WINDOW * gameWindow = newwin( 24,42*2,0,0 );

    // board creation and setup
    Tile ** gameBoard = createBoard( boardSettings );
    
    boardSettings->mineCount = calculateMineCount( boardSettings );
    

    gameBoard = plotMines( gameBoard, boardSettings );
    gameBoard = plotNumbers( gameBoard, boardSettings );
    
    PlayerSelection selection;

    selection.xSelection = 0;
    selection.ySelection = 0;

    renderBoard( gameWindow, gameBoard, boardSettings, selection );

    while ( 1 )
    {

        selection = gameInput( gameWindow, selection, boardSettings );

        if ( selection.action == UNCOVER )
        {
            if ( !gameBoard[ selection.ySelection ][ selection.xSelection ].flagged )
            {
                //uncover
                gameBoard[ selection.ySelection ][ selection.xSelection ].visible = 1;
            } 
        }

        if ( selection.action == FLAG )
        {
            if ( !gameBoard[ selection.ySelection ][ selection.xSelection ].flagged )
            {
                gameBoard[ selection.ySelection ][ selection.xSelection ].flagged = 1;
            }
            else
            {
                gameBoard[ selection.ySelection ][ selection.xSelection ].flagged = 0;
            }
        }

        renderBoard( gameWindow, gameBoard, boardSettings, selection );
    }

    delwin( gameWindow );
}

//
// Create Board
// allocate memory for the board and set all tile feilds to 0
//

Tile ** createBoard( GameSettings * boardSettings )
{
    Tile ** board = ( Tile ** ) malloc( sizeof( Tile * ) * boardSettings->ySize );

    for ( int i = 0; i < boardSettings->ySize; i++ )
    {
        board[ i ] = ( Tile * ) malloc( sizeof( Tile ) * boardSettings->xSize );
    }

    // this is done mainly to make sure there's no left over memory being used
    for ( int i = 0; i < boardSettings->ySize; i++ )
    {
        for ( int j = 0; j < boardSettings->xSize; j++ )
        {
            board[ i ][ j ].clicked = 0;
            board[ i ][ j ].flagged = 0;
            board[ i ][ j ].mine = 0;
            board[ i ][ j ].number = 0;
            board[ i ][ j ].visible = 0;
        }
    }

    return board;
}

//
// Delete Board
// free's the boards memory after the game is done
//

void deleteBoard( Tile ** board, GameSettings * boardSettings )
{
    for ( int i = 0; i < boardSettings->ySize; i++ ) free( board[ i ] );
    free( board );

    board = NULL;
}

//
// Game Input
// handles input from the user
//

PlayerSelection gameInput( WINDOW * win, PlayerSelection choice, GameSettings * bounds )
{
    choice.action = 0;

    int ch = wgetch( win );

    switch (ch)
    {
        case 'w':
            if (choice.ySelection > 0) choice.ySelection--;
            break;

        case 'a':
            if ( choice.xSelection > 0 ) choice.xSelection--;
            break;

        case 's':
            if ( choice.ySelection < bounds->ySize - 1 ) choice.ySelection++;
            break;

        case 'd':
            if ( choice.xSelection < bounds->xSize - 1 ) choice.xSelection++;
            break;

        case ' ':
        case 'o':
        case 'e':
            choice.action = UNCOVER;
            break;

        case 'p':
        case 'f':
            choice.action = FLAG;
            break;

        case 27:
            choice.action = BACK;
            break;
    }

    return choice;
}

//
// Render Board
// renders out the game board on every input
//

void renderBoard( WINDOW * win, Tile ** board, GameSettings * boardSettings, PlayerSelection selection )
{
    int xRenderSize = min( boardSettings->xSize * 2, 40 * 2 ); //TODO: replace these hardcoded values with their respective max values based on screen size
    int yRenderSize = min( boardSettings->ySize, 22 );

    for ( int i = 0; i < yRenderSize; i++ )
    {
        for ( int j = 0; j < xRenderSize; j++ )
        {
            if ( i == selection.ySelection && j == selection.xSelection ) wattron( win, A_STANDOUT );

            if ( board[ i ][ j ].flagged )
            {
                wattron( win, COLOR_PAIR( COLOR_FLAG ) );
                mvwprintw( win, i + 1, j * 2 + 2, "<\\" );
                wattroff( win, COLOR_PAIR( COLOR_FLAG ) );
            }
            else if ( !board[ i ][ j ].visible )
            {
                wattron( win, COLOR_PAIR( COLOR_COVERED ) );
                mvwprintw( win, i + 1, j * 2 + 2, "[]" );
                wattroff( win, COLOR_PAIR( COLOR_COVERED ) );
            } 
            else if ( board[ i ][ j ].mine )
            {
                wattron( win, COLOR_PAIR( COLOR_MINE ) );
                mvwprintw( win, i + 1, j * 2 + 2, "<>" );
                wattroff( win, COLOR_PAIR( COLOR_MINE ) );
            }
            else if ( board[ i ][ j ].number )
            {
                wattron( win, COLOR_PAIR( board[ i ][ j ].number ) );
                mvwprintw( win, i + 1, j * 2 + 2, " %d", board[ i ][ j ].number );
                wattroff( win, COLOR_PAIR( board[ i ][ j ].number ) );
            }
            else
            {
                wattron( win, COLOR_PAIR( COLOR_UNCOVERED ) );
                mvwprintw( win, i + 1, j * 2 + 2, "  " );
                wattroff( win, COLOR_PAIR( COLOR_UNCOVERED ) );
            }

            wattroff( win, A_STANDOUT );
        }
    }
    
    // draw border and show to screen
    box( win, 0, 0 );

    wrefresh( win );
}

//
// Calculate Mine Count
// calculates the number of mines based off of the mine ratio unless useMineCount is selected
//

int calculateMineCount( GameSettings * boardSettings )
{
    const float suggestedMineRatio = 0.21;

    if ( boardSettings->useMineCount == 1 )
    {
        return boardSettings->mineCount;
    }
    else if ( boardSettings->useSuggestedMineRatio == 1 )
    {
        return boardSettings->ySize * boardSettings->xSize * suggestedMineRatio;
    }
    else
    {
        return boardSettings->ySize * boardSettings->xSize * boardSettings->mineRatio;
    }
}

//
// Plot Mines
// randomly places mines around the board
//

Tile ** plotMines( Tile ** board, GameSettings * boardSettings )
{
    int mines = boardSettings->mineCount;

    while ( mines )
    {
        int yMinePos = rand() % ( boardSettings->ySize - 1 );
        int xMinePos = rand() % ( boardSettings->xSize - 1 );

        if ( !board[ yMinePos ][ xMinePos ].mine )
        {
            board[ yMinePos ][ xMinePos ].mine = 1;
            mines--;
        }
    }

    return board;
}

//
// Plot Numbers
// finds every mine and then adds 1 to each of their surrounding areas
//

Tile ** plotNumbers( Tile ** board, GameSettings * boardSettings )
{
    // loop through all tiles
    for ( int i = 0; i < boardSettings->ySize; i++ )
    {
        for ( int j = 0; j < boardSettings->xSize; j++ )
        {
            // if you find a mine, +1 to all the surrounding tiles
            // * this technically means that mines also count the same tile that they're in but that doesn't matter really
            if ( board[ i ][ j ].mine )
            {
                for ( int k = i - 1; k <= i + 1; k++ )
                {
                    for ( int l = j - 1; l <= j + 1; l++ )
                    {
                        if ( !( k < 0 || k >= boardSettings->ySize || l < 0 || l >= boardSettings->xSize ) )
                            board[ k ][ l ].number++;
                    }
                }
            }
        }
    }

    return board;
}

//
// Open Area
// makes an area the user clicked on visible
//

Tile ** openArea( Tile ** board, PlayerSelection selection )
{
    TileQueue queue;
    queue.next = NULL;

    enqueueTile( &queue, selection );

    // remove locations from the queue until empty
    while ( queue.next != NULL )
    {
        // get a current location
        PlayerSelection currentBoardPosition = dequeueTile( &queue );

        for (int i = currentBoardPosition.ySelection - 1; i < currentBoardPosition.ySelection + 1; i++)
        {
            for ( int j = currentBoardPosition.xSelection - 1; j < currentBoardPosition.xSelection + 1; j++ )
            {
                if ( board[ i ][ j ].visible ) continue;

                if ( board[ i ][ j ].number ) 
                {
                    board[ i ][ j ].visible = 1;
                    continue;
                }

                board[ i ][ j ].visible = 1;

                PlayerSelection nextBoardSelection;
                nextBoardSelection.ySelection = i;
                nextBoardSelection.xSelection = j;

                enqueueTile( &queue, nextBoardSelection );
            }
        }

        usleep( 1000 * 100 );
    }

    return board;
}



void enqueueTile( TileQueue * queue, PlayerSelection selection )
{
    TileQueue * queuedTile = ( TileQueue * ) malloc( sizeof( TileQueue ) );

    queuedTile->selection = selection;
    queuedTile->next = queue->next;
    queue->next = queuedTile;

    // TODO: finish this
}

PlayerSelection dequeueTile( TileQueue * queue )
{
    // TODO: finish this

    return ;
}

//
// Min
// returns the minimum of two integers
//

int min( int a, int b )
{
    if ( a < b ) return a;
    else return b;
}

//==================================================================================================//
