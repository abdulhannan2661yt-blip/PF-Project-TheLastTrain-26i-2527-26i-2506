#include "switches.h"

// TODO: Lift 1 only works between floor 1 and floor 2
bool canUseLift1(int fromFloor,int toFloor)
{
return false;
}

// TODO: Lift 2 links floors 2, 4 and 5; needs lift2Operational or emergency trips left
bool canUseLift2(int fromFloor,int toFloor,bool lift2Operational,int emergencyTripsLeft)
{
return false;
}

// TODO: return true if the Master Key is in the inventory
bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount)
{
return false;
}

// TODO: remove the fuse from the inventory, mark it installed, add score
bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,
bool objectives[NUM_OBJECTIVES],int& score)
{
return false;
}

// TODO: code must be "427" and all three clues must be found
bool verifyCabinetCode(const char* enteredCode,const bool objectives[NUM_OBJECTIVES])
{
return false;
}
