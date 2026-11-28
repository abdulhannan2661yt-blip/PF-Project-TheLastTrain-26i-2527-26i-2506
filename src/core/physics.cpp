#include "physics.h"
#include "util.h"
using namespace std;

static bool IsInRange(int Floor, int Room)
{
return ((Floor >= 1) && (Floor <= NUM_FLOORS)) && ((Room >= 1) && (Room <= ROOMS_PER_FLOOR));
}

static bool IsLift1Floor(int Floor) 
{ 
return Floor == 1 || Floor == 2; 
}
static bool IsLift2Floor(int Floor) 
{
return Floor == 2 || Floor == 4 || Floor == 5; 
}
static bool IsLift1Room(int Room)   
{ 
return Room == 2 || Room == 3; 
}

bool isDirectlyConnected(int FromFloor,int FromRoom,int ToFloor,int ToRoom,const char HorizontalConnection[NUM_FLOORS][ROOMS_PER_FLOOR - 1],char& OutConnectionType)
{
OutConnectionType=CONN_NONE;
if(!IsInRange(FromFloor,FromRoom) || !IsInRange(ToFloor,ToRoom))
return false;
if (FromFloor==ToFloor)
{
int Difference=FromRoom-ToRoom;
if((Difference!=1) && (Difference!=-1))
return false;
int Lower=(FromRoom < ToRoom) ? FromRoom : ToRoom;
char Connector=HorizontalConnection[FromFloor-1][Lower-1];
if((Connector==CONN_DOOR) || (Connector==CONN_WINDOW) || (Connector==CONN_LOCKED_DOOR))
{
OutConnectionType=Connector;
return true;
}
if(Connector==CONN_SOLID_WALL)
OutConnectionType=CONN_SOLID_WALL;
return false;
}
if ((FromRoom == 1) && (ToRoom == 1)) 
{
int LowFloor = (FromFloor < ToFloor) ? FromFloor : ToFloor;
int HighFloor = (FromFloor < ToFloor) ? ToFloor : FromFloor;
if ((HighFloor - LowFloor == 1) && (LowFloor == 1 || LowFloor == 2 || LowFloor == 4))
{
OutConnectionType = CONN_STAIRS;                 
return true;
}
}

if((IsLift1Floor(FromFloor)) && (IsLift1Floor(ToFloor)) && (IsLift1Room(FromRoom)) && (IsLift1Room(ToRoom)))
{
OutConnectionType=CONN_LIFT1;
return true;
}
if ((IsLift2Floor(FromFloor)) && (IsLift2Floor(ToFloor)) && (FromRoom == 4) && (ToRoom == 4)) 
{
OutConnectionType=CONN_LIFT2;
return true;
}
return false;
}

bool canTraverse(int TargetFloor,int TargetRoom,char ConnectionType,const char Inventory[MAX_INVENTORY_CAPACITY],int InventoryCount,int EmergencyTripsLeft,bool Lift2_Operational)
{
if(!IsInRange(TargetFloor,TargetRoom))
return false;
switch(ConnectionType)
{
case (CONN_DOOR):
case (CONN_WINDOW):
case (CONN_STAIRS):
case (CONN_LIFT1):
return true;
case (CONN_LOCKED_DOOR):
return hasItem(Inventory,InventoryCount,ENT_MASTER_KEY);
case (CONN_LIFT2):
if(Lift2_Operational)
return true;
return (EmergencyTripsLeft > 0) && (hasItem(Inventory, InventoryCount, ENT_EMERGENCY_CARD));
default:
return false;
}
}

void applyResourceDelta(int& Energy,int& Time,int DeltaEnergy,int DeltaTime)
{
Energy = clampInt(Energy + DeltaEnergy, 0, MAX_ENERGY);
Time   = clampInt(Time + DeltaTime, 0, MAX_TIME);
return;
}

void applyScoreDelta(int& Score,int DeltaScore)
{
Score=Score+DeltaScore;
if(Score<0)
Score=0;
}

bool hasItem(const char Inventory[MAX_INVENTORY_CAPACITY],int InventoryCount,char ItemSymbol)
{
for(int i=0;(i<InventoryCount) && (i<MAX_INVENTORY_CAPACITY);i++)
{
if (Inventory[i]==ItemSymbol)
return true;
}
return false;
}

bool addItem(char Inventory[MAX_INVENTORY_CAPACITY],int& InventoryCount,char ItemSymbol)
{
if ((InventoryCount<0) || (InventoryCount >= MAX_INVENTORY_CAPACITY))
return false;
Inventory[InventoryCount]=ItemSymbol;
InventoryCount++;
return true;
}

bool removeItem(char Inventory[MAX_INVENTORY_CAPACITY],int& InventoryCount,char ItemSymbol)
{
for(int i=0;i<InventoryCount;i++)
{
if(Inventory[i]==ItemSymbol)
{
for (int j = i; j < InventoryCount - 1; j++) Inventory[j] = Inventory[j + 1];
InventoryCount--;
return true;
}
}
return false;
}

