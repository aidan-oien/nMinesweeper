#define MENU_STRING_LENGTH 30

//==================================================================================================//

typedef struct GameResults GameResults;
typedef struct GameSettings GameSettings;
typedef struct PlayerSelection PlayerSelection;

struct GameSettings
{
	short xSize;
	short ySize;

    float mineRatio;

    char useMineCount;
	int mineCount;

    char useSuggestedMineRatio;
};

struct GameResults
{
    float finalTime;
    char difficulty[ MENU_STRING_LENGTH ];
	int xBoardSize;
	int yBoardSize;
    int mineCount;
};

struct PlayerSelection
{
	char action;
    short xSelection;
	short ySelection;
};

enum colorDescriptors
{
	COLOR_MINE = 9,
	COLOR_FLAG,
	COLOR_COVERED,
	COLOR_UNCOVERED,
    COLOR_MENU_ACTIVE,
	COLOR_MENU_INACTIVE
};

//==================================================================================================//