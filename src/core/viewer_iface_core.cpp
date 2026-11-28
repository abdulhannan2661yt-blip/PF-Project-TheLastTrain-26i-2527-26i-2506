#include "viewer_iface.h"
#include "util.h"
#include "level.h"
#include <iostream>
using namespace std;

void renderDashboard(int playerFloor,int playerRoom,int energy,int time,int score,const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,int emergencyTripsLeft)
{
cout<<"Player is currently in F"<<playerFloor<<"-R" << playerRoom << endl;
cout<<"Energy: "<<energy<<"/"<<MAX_ENERGY<<endl;
cout<<"Time: "<<time<<"/"<<MAX_TIME<<endl;
cout<<"Score: "<<score<<endl;
cout<<"Inventory: ";
if(inventoryCount==0)
cout<<"empty";
for (int i = 0;i < inventoryCount;i++)
cout<<inventory[i]<<" ";
cout<<endl;
cout<<"Emergency Trips: "<<emergencyTripsLeft<<endl;
}

void renderRoomDetails(int floor,int room,const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCount)
{
cout<<"Room F"<<floor<<"-R"<<room<<" ("<<(getRoomName(floor,room))<<") contains:"<<endl;
if (entityCount==0)
{
cout<<"No items"<<endl;
return;
}
for (int i=0;i<entityCount;i++)
{
char symbol= roomEntities[floor-1][room-1][i];
cout<<" "<<symbol<<" "<< getEntityName(symbol)<<endl;
}
}
void renderAsciiBuilding(int playerFloor,int playerRoom)
{
static const char* Connectors[NUM_FLOORS][ROOMS_PER_FLOOR-1]={  { " W ", " L1", " D " } , { " D ", " L1", " D " } , { " L ", " W ", " D " } , { " W ", " # ", " D " } , { " D ", " D ", " W " } };
static const char* Lifts[NUM_FLOORS] = { "", " L2", "", " L2", " L2" };
cout << "Map of Building (D=Door, W=Window, L=Locked, #=Wall, L1/L2=Lifts, S=Stairs at R1)"<<endl;
for(int Floor=NUM_FLOORS; Floor>=1;Floor--)
{
cout<<" ";
for(int Room=1;Room<=ROOMS_PER_FLOOR;Room++)
{
bool Present=(Floor==playerFloor && Room==playerRoom);
cout<<(Present ? "[": " ")<<"F"<<Floor<<"-R"<<Room<<(Present ? "]": " ");
if (Room < ROOMS_PER_FLOOR) 
cout << Connectors[Floor - 1][Room - 1];
}    
cout<<Lifts[Floor-1]<<endl;
}
}

void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES])
{
const char* Names[]={"Master Key Obtained", "Clue's 4th digit Found", "Clue's 2nd digit Found", "Clue's 7th digit Found","Cabinet Unlocked Successfully (427)", "Train Key Obtained", "Fuse Collected", "Fuse Inserted at panel","Collected Lift Pass ", "Lift 2 completely working", "Passenger brought to safety", "Cat Carrier collected successfully","Cat Rescued", "Train is ready for Boarding", "Mission Accomplished", "Mission Failed: Try Again"};
printHeader("Objectives Completed:");
for(int i=0;i<NUM_OBJECTIVES;i++)
if (objectives[i]) 
cout << "  [X] " << Names[i] << endl;
else
cout << "  [ ] " << Names[i] <<endl;
}