#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "level.h"
#include "viewer_iface.h"
#include "util.h"
#include <iostream>
using namespace std;

// TODO: main game loop: show status, read a command, call the right handler, check win/lose
void runSimulation(int& playerFloor,int& playerRoom,
int& energy,int& time,int& score,
char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,
bool objectives[NUM_OBJECTIVES],int& emergencyTripsLeft,
char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
cout<<"Simulation not written yet."<<endl;
}

// TODO: move the player if the rooms are connected and allowed; take energy/time
bool handleMoveCommand(int targetFloor,int targetRoom,
int& currentFloor,int& currentRoom,
int& energy,int& time,
const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,
int& emergencyTripsLeft,bool lift2Operational)
{
return false;
}

// TODO: search the room, reveal clue digits, take energy/time
bool handleInspectCommand(int currentFloor,int currentRoom,
int& energy,int& time,
const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
int entityCount,
bool objectives[NUM_OBJECTIVES],int& score)
{
return false;
}

// TODO: pick up an item from the room into the inventory, apply bonuses/score
bool handleCollectCommand(char itemSymbol,int currentFloor,int currentRoom,
int& energy,int& time,int& score,
char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
int& entityCount,
char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,
bool objectives[NUM_OBJECTIVES])
{
return false;
}

// TODO: use an item (fuse at F3-R4, medical kit at F4-R1, train key at F5-R2)
bool handleUseCommand(char itemSymbol,int currentFloor,int currentRoom,
int& energy,int& time,int& score,
char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,
bool objectives[NUM_OBJECTIVES])
{
return false;
}

// TODO: open the cabinet in F5-R1 with code 427
bool handleUnlockCabinetCommand(const char* code,int currentFloor,int currentRoom,
bool objectives[NUM_OBJECTIVES],int& score,
char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
int& entityCount)
{
return false;
}

// TODO: return true if the game is over (energy or time is 0, or the mission is won)
bool checkEndConditions(int energy,int time,const bool objectives[NUM_OBJECTIVES])
{
return false;
}
