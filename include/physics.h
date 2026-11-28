#ifndef PHYSICS_H
#define PHYSICS_H

#include "state.h"

const char CONN_STAIRS = 'S';
const char CONN_LIFT1  = '1';
const char CONN_LIFT2  = '2';
// Movement validation, connectivity & resource adjustments
bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char& outConnType);

bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational);

void applyResourceDelta(int& energy, int& time, int deltaEnergy, int deltaTime);
void applyScoreDelta(int& score, int deltaScore);

bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol);
bool addItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol);
bool removeItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol);

#endif // PHYSICS_H
