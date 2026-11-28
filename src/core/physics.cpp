#include "physics.h"
#include "util.h"
using namespace std;

// TODO: check if two rooms are connected (door, window, locked door, stairs, lift) and store the type in outConnType
bool isDirectlyConnected(int fromF,int fromR,int toF,int toR,
const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
char& outConnType)
{
return false;
}

// TODO: check if the player is allowed through this connection (locked door needs the Master Key, Lift 2 rules)
bool canTraverse(int targetFloor,int targetRoom,char connType,
const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,
int emergencyTripsLeft,bool lift2Operational)
{
return false;
}

// TODO: add the deltas to energy and time, then keep them inside their limits
void applyResourceDelta(int& energy,int& time,int deltaEnergy,int deltaTime)
{
}

// TODO: add deltaScore to score
void applyScoreDelta(int& score,int deltaScore)
{
}

// TODO: return true if itemSymbol is in the inventory
bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,char itemSymbol)
{
return false;
}

// TODO: add itemSymbol to the inventory if there is space
bool addItem(char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,char itemSymbol)
{
return false;
}

// TODO: remove itemSymbol from the inventory and shift the rest left
bool removeItem(char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,char itemSymbol)
{
return false;
}
