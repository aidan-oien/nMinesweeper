#define MENU_STRING_LENGTH 30

//==================================================================================================//

typedef struct gameResults gameResults;
typedef struct gameSettings gameSettings;

struct gameSettings
{
	short xSize;
	int ySize;

    float mineRatio;

    char useMineCount;
	int mineCount;

    char useSuggestedMineRatio;
};

struct gameResults
{
    float finalTime;
    char win;
};

//==================================================================================================//

int min( int a, int b )
{
    if ( a < b ) return a;
    else return b;
}