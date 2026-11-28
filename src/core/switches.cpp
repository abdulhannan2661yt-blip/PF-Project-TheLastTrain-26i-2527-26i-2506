#include "switches.h"
#include "physics.h"
#include "util.h"
#include <cstring>
using namespace std;

bool canUseLift1(int fromFloor,int toFloor)
{
    bool fromOk=(fromFloor==1 || fromFloor==2);
    bool toOk=(toFloor==1 || toFloor==2);
    return fromOk && toOk &&fromFloor!=toFloor;
}

bool canUseLift2(int fromFloor,int toFloor,bool lift2Operational,int emergencyTripsLeft)
{
    bool fromOk=(fromFloor==2 || fromFloor==4 || fromFloor==5);
    bool toOk=(toFloor==2 || toFloor==4 || toFloor==5);
    if (!fromOk||!toOk||fromFloor==toFloor)
        return false;
    return lift2Operational||emergencyTripsLeft > 0;
}

bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount)
{
    return hasItem(inventory, inventoryCount, ENT_MASTER_KEY);
}

bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES],int& score)
{
    if (objectives[OBJ_FUSE_INSTALLED])
        if (!hasItem(inventory, inventoryCount, ENT_FUSE))
            return false;
    removeItem(inventory, inventoryCount, ENT_FUSE);
    objectives[OBJ_FUSE_INSTALLED] = true;
    applyScoreDelta(score, SCORE_INSTALL_FUSE);
    
    if (hasItem(inventory, inventoryCount, ENT_LIFT_PASS)) {
        objectives[OBJ_LIFT2_ACTIVE] = true;
    }
    return true;
}
bool verifyCabinetCode(const char* enteredCode,const bool objectives[NUM_OBJECTIVES])
{
    if (enteredCode == nullptr)
     return false;
    if (!objectives[OBJ_CLUE_4] || !objectives[OBJ_CLUE_2] || !objectives[OBJ_CLUE_7]) return false;
    return strcmp(enteredCode, "427")==0;
}
