#include <ncurses.h>
#include <stdlib.h>
#include "shared.h"

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

gameResults * startGame( gameSettings * boardSettings )
{
    WINDOW * gameWindow = newwin( 24,42,0,0 );

    // board creation and setup
    cell ** gameBoard = createBoard( boardSettings );
    gameBoard = calculateMineCount( boardSettings );
    // gameBoard = plotNumbers( cell ** gameBoard );

    renderBoard( gameWindow, gameBoard, boardSettings );

    while ( 1 )
    {

        gameInput();
        renderBoard( gameWindow, gameBoard, boardSettings );
    }

    delwin( gameWindow );
}

cell ** createBoard( gameSettings * boardSettings )
{
    // allocate memory to make a board
    cell ** board = ( cell ** ) malloc( sizeof( cell * ) * boardSettings->ySize );

    for ( int i = 0; i < boardSettings->ySize; i++ )
    {
        cell * board = ( cell * ) malloc( sizeof( cell ) * boardSettings->xSize );
    }

    // this is done mainly to make sure there's no left over memory being used
    for ( int i = 0; i < boardSettings->ySize; i++ )
    {
        for ( int j = 0; j < boardSettings->xSize; j++ )
        {
            board[ j ][ i ].clicked = 0;
            board[ j ][ i ].flagged = 0;
            board[ j ][ i ].mine = 0;
            board[ j ][ i ].number = 0;
            board[ j ][ i ].visible = 0;
        }
    }

    // give newly created board
    return board;
}

void deleteBoard( cell ** board, gameSettings * boardSettings )
{
    // free all of the memory from board
    for ( int i = 0; i < boardSettings->ySize; i++ ) free( board[ i ] );
    free( board );

    // set board to NULL to make sure it's not pointing to random data
    board = NULL;
}

void renderBoard( WINDOW * win, cell ** board, gameSettings * boardSettings )
{
    int xRenderSize = min( boardSettings->xSize, 40 ); //TODO: replace these hardcoded values with their respective max values based on screen size
    int yRenderSize = min( boardSettings->ySize, 22 );

    for ( int i = 0; i < yRenderSize; i++)
    {
        for (int j = 0; j < xRenderSize; j++)
        {
            if ( board[ j ][ i ].flagged )
            {
                wattron( win, COLOR_PAIR( 1 ) );
                mvwprintw( win, i * 2, j, "<\\" );
                wattroff( win, COLOR_PAIR( 1 ) );
            }
            else if ( !board[ j ][ i ].visible )
            {
                wattron( win, COLOR_PAIR( 11 ) );
                mvwprintw( win, i * 2, j, "  " );
                wattroff( win, COLOR_PAIR( 11 ) );
            } 
            else if ( board[ j ][ i ].mine )
            {
                wattron( win, COLOR_PAIR( 9 ) );
                mvwprintw( win, i * 2, j, "<>" );
                wattroff( win, COLOR_PAIR( 9 ) );
            }
            else if ( board[ j ][ i ].number )
            {
                wattron( win, COLOR_PAIR( board[ j ][ i ].number ) );
                mvwprintw( win, i * 2, j, " %d", board[ j ][ i ].number );
                wattroff( win, COLOR_PAIR( board[ j ][ i ].number ) );
            }
            else
            {
                wattron( win, COLOR_PAIR( board[ j ][ i ].number ) );
                mvwprintw( win, i * 2, j, "  " );
                wattroff( win, COLOR_PAIR( board[ j ][ i ].number ) );
            }
        }
    }
    
    // draw border and show to screen
    box( win, 0, 0 );
    wrefresh( win );
}

int calculateMineCount( gameSettings * boardSettings )
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

int min( int a, int b )
{
    if ( a < b ) return a;
    else return b;
}

//==================================================================================================//
