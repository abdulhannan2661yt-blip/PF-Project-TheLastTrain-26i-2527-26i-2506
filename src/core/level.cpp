#include "level.h"
#include <iostream>
#include <fstream>
#include "util.h"
using namespace std;

static const char* ROOM_DESIGNATION[NUM_FLOORS][ROOMS_PER_FLOOR] = {
    {"Entrance lobby","Reception", "Waiting area", "Utility Room"},
    {"Lounge", "Archive", "Lab", "Meeting Room"},
    {"Stairwell annex", "Security vestibule", "Animal Room", "Control Room"},
    {"First aid Station","Medical Room" ,"Maintenance corridor", "Office" },
    {"Security Room", "Train boarding platform", "Storage Room", "Observation Deck"}
};
const char* getRoomName(int floor,int room)
{
if(floor<1||floor>NUM_FLOORS||room<1||room>ROOMS_PER_FLOOR){
return "Unknown Room";
}
return ROOM_DESIGNATION[floor-1][room-1];
}

const char* getEntityName(char entitySymbol){
    switch (entitySymbol){
        case ENT_MASTER_KEY:
            return "Master Key";
        case ENT_ENERGY_PACK:
            return "Energy Pack";
        case ENT_EMERGENCY_TOOLKIT:
            return "Emergency Toolkit";
        case ENT_CLUE_4:
            return "Clue digit 4";
        case ENT_FUSE:
            return "Fuse";
        case ENT_CAT:
            return "cat";
        case ENT_CONTROL_PANEL:
            return "Control Panel";
        case ENT_LIFT_PASS:
            return "Lift Pass";
        case ENT_PASSENGER:
            return "injured passenger";
        case ENT_MEDICAL_KIT:
            return "Medical kit";
        case ENT_EMERGENCY_CARD:
            return "Emergency access Card";
        case ENT_CLUE_2:
            return "Clue Digit 2";
        case ENT_CLUE_7:
            return "Clue Digit 7";
        case ENT_CABINET_TRAIN_KEY:
            return "cabinet / Train key";
        case ENT_CAT_CARRIER:
            return "Cat Carrier";
        case ENT_OBSERVATION_TOKEN:
            return "Observation token";
        default:
            return "Unknown Item";
    }
}
bool isKnownEntity(char c)
{
return !stringEquals(getEntityName(c),"Unknown Item");
}

int findRoomLabel(const char* line,int floor, int room){
    if(line==nullptr){
        return -1;
    }
    char fd='0'+ floor;
    char rd='0' + room;
    int i=0;
    while(line[i]!='\0'){
        if(line[i]=='F'&&line[i+1]==fd&&line[i+2]=='-'&&line[i+3]=='R'&&line[i+4]==rd){
            return i;
        }
        i++;
    }
    return -1;
}
bool findBorderColumns(char** lines,int lineCount,int plusCols[ROOMS_PER_FLOOR+1]){
    for(int i=0; i<lineCount; i++){
        int count=0;
        int temp[ROOMS_PER_FLOOR+1];
        int j=0;
        while (lines[i][j]!='\0'){
            if (lines[i][j]=='+'){
                if(count<ROOMS_PER_FLOOR+1){
                    temp[count]=j;
                }
                count++;
            }
            j++;
        }
        if(count==ROOMS_PER_FLOOR+1){
            for(int k=0; k<ROOMS_PER_FLOOR+1; k++){
                plusCols[k]=temp[k];
            }
            return true;
        }
    }
    return false;
}

bool readConnector(const char* text,char&outType){
    if(text==nullptr){
        return false;
    }
    int i=0;
    while(text[i]==' '){
        i++;
    }
    char word[4]={'\0','\0','\0','\0'};
    int len=0;
    while(text[i]!='\0'&&text[i]!=' '&&text[i]!='|'&&len<3){
        word[len]=text[i];
    len++;
    i++;
}
if(len==1){
    if(word[0]=='D' || word[0]=='W' || word[0]=='L' || word[0]=='#'){
        outType=word[0];
        return true;
    }
}
if(len==2&&word[0]=='L'&&word[1]=='1'){
outType=CONN_NONE;
return true;
}
return false;
}

bool addEntity(char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR],int f,int r,char symbol)
{
if(f<0||f>=NUM_FLOORS||r<0||r>=ROOMS_PER_FLOOR){
return false;
}
if(entityCounts[f][r]>=MAX_ENTITIES_PER_ROOM){
return false;
}
int slot=entityCounts[f][r];
roomEntities[f][r][slot]=symbol;
entityCounts[f][r]++;
return true;
}

bool findFloorLabels(const char* line,int floor,int cols[ROOMS_PER_FLOOR]){
    for(int r=1; r<=ROOMS_PER_FLOOR; r++){
        cols[r-1]=findRoomLabel(line,floor,r);
        if(cols[r-1]==-1){
            return false;
        }
    }
    return true;
}
int lineLength(const char* s){
    int n=0;
    while(s[n]!='\0'){
        n++;
    }
    return n;
}
void clearMapArrays(char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR-1],char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    for(int f=0; f<NUM_FLOORS; f++){
        for(int c=0; c<ROOMS_PER_FLOOR-1; c++){
            horizontalConn[f][c]=CONN_NONE;
        }
        for(int r=0;r<ROOMS_PER_FLOOR;r++){
            entityCounts[f][r]=0;
            for(int e=0;e<MAX_ENTITIES_PER_ROOM;e++){
                roomEntities[f][r][e]=ENT_NONE;
            }
        }
    }
}
bool parseMapLines(char** lines,int lineCount,char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR-1],char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    clearMapArrays(horizontalConn,roomEntities,entityCounts);
    int plusCols[ROOMS_PER_FLOOR+1];
    if (!findBorderColumns(lines,lineCount,plusCols)){
        cout << "Map error: border line not found." << endl;
        return false;
    }
    bool floorFound[NUM_FLOORS]={};
    for(int i=0;i<lineCount;i++){
        for(int floor=1;floor<=NUM_FLOORS;floor++){
            int labelCols[ROOMS_PER_FLOOR];
            if(floorFound[floor-1]){
                continue;
            }
            if(!findFloorLabels(lines[i],floor,labelCols)){
                continue;
            }
            floorFound[floor-1]=true;
            for(int r=1;r<ROOMS_PER_FLOOR;r++){
                char type=CONN_NONE;
                if(!readConnector(lines[i]+labelCols[r-1]+5,type)){
                    cout<<"Map error:bad connection after F"<<floor<<"-R"<<r<<endl;
                    return false;
                }
                horizontalConn[floor-1][r-1]=type;
            }
            if(i+1>=lineCount){
                cout << "Map error: item line missing for floor" << floor << endl;
                return false;
            }
            char* itemLine=lines[i+1];
            int itemLen=lineLength(itemLine);
            for(int r=1; r<=ROOMS_PER_FLOOR; r++){
                int from=plusCols[r-1]+1;
                int to=plusCols[r]-1;
                for(int c=from; c<=to && c<itemLen; c++){
                    char sym=itemLine[c];
                    if(sym==' ' || sym=='|'){
                        continue;
                    }
                    if(!isKnownEntity(sym)){
                        cout << "Map error: unknown symbol" << sym <<" i F" << floor<< "-R" << r << endl;
                        return false;
                    }
                    int copies=1;
                    if(sym==ENT_ENERGY_PACK){
                        copies=2;
                    }
                    for(int k=0;k<copies; k++){
                        if(!addEntity(roomEntities,entityCounts,floor-1,r-1,sym)){
                            cout << "Map error: too many item in F" << floor << "-R" << r << endl;
                            return false;
                        }
                    }
                }
            }
        }
    }
    for(int f=0; f<NUM_FLOORS; f++){
        if(!floorFound[f]){
            cout << "Map error: floor " << f+1 << "not found." <<endl;
            return false;
        }
    }
    return true;
}
bool loadBuildingMap(const char* filename,char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR-1],char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
ifstream file(filename);
if(!file.is_open()){
cout<<"Error: cannot open "<<filename<<endl;
return false;
}
const int MAX_MAP_LINES=60;
char** lines=new char*[MAX_MAP_LINES];
int lineCount=0;
char buffer[MAX_LINE_LEN];
while(lineCount<MAX_MAP_LINES&&file.getline(buffer,MAX_LINE_LEN)){
int len=lineLength(buffer);
while(len>0&&(buffer[len-1]=='\r'||buffer[len-1]=='\n')){
len--;
buffer[len]='\0';
}
lines[lineCount]=new char[len+1];
for(int k=0;k<=len;k++){
lines[lineCount][k]=buffer[k];
}
lineCount++;
    }
file.close();
bool ok=parseMapLines(lines,lineCount,horizontalConn,roomEntities,entityCounts);
for(int i=0;i<lineCount;i++){
delete[] lines[i];
   }
delete[] lines;
return ok;
}

bool readYesNo(const char* value,bool& out)
{
if(value==nullptr){
return false;
}
if(stringEquals(value,"YES")||stringEquals(value,"TRUE")){
out=true;
return true;
}
if(stringEquals(value,"NO")||stringEquals(value,"FALSE")){
out=false;
return true;
}
return false;
}


bool splitKeyValue(char* line,char*& key,char*& value)
{
int i=0;
while(line[i]!='\0'&&line[i]!=':'){
i++;
}
if(line[i]!=':'){
return false;
}
line[i]='\0';
key=line;
value=line+i+1;
trimWhitespace(key);
trimWhitespace(value);
return true;
}

bool setFlagFromKey(const char* key,const char* value,bool objectives[NUM_OBJECTIVES],bool& valid)
{
const int FLAG_COUNT=7;
const char* flagKeys[FLAG_COUNT]={"PASSENGER_RESCUED","CAT_RESCUED","CABINET_UNLOCKED","TRAIN_KEY_COLLECTED","FUSE_INSTALLED","LIFT_PASS_COLLECTED","TRAIN_PREPARED"};
int flagIndex[FLAG_COUNT]={OBJ_PASSENGER_RESCUED,OBJ_CAT_SECURED,OBJ_CABINET_UNLOCKED,OBJ_TRAIN_KEY,OBJ_FUSE_INSTALLED,OBJ_LIFT_PASS,OBJ_TRAIN_PREPARED};
for(int k=0;k<FLAG_COUNT;k++){
if(stringEquals(key,flagKeys[k])){
bool answer=false;
valid=readYesNo(value,answer);
if(valid){
objectives[flagIndex[k]]=answer;
}
return true;
}
}
return false;
}


bool loadInitialState(const char* filename,int& playerFloor,int& playerRoom,int& energy,int& time,int& score,char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES],int& emergencyTripsLeft)
{
playerFloor=0;
playerRoom=0;
energy=INITIAL_ENERGY;
time=INITIAL_TIME;
score=INITIAL_SCORE;
inventoryCount=0;
emergencyTripsLeft=0;
for(int i=0;i<NUM_OBJECTIVES;i++){
objectives[i]=false;
    }
ifstream file(filename);
if(!file.is_open()){
cout<<"Error: cannot open "<<filename<<endl;
return false;
   }
char line[MAX_LINE_LEN];
while(file.getline(line,MAX_LINE_LEN)){
trimWhitespace(line);
if(line[0]=='\0'){
continue;
     }
char* key=nullptr;
char* value=nullptr;
if(!splitKeyValue(line,key,value)){
continue;
   }
bool valid=true;
int number=0;
if(setFlagFromKey(key,value,objectives,valid)){
if(!valid){
cout<<"State error: "<<key<<" must be YES or NO"<<endl;
return false;
     }
}
else if(stringEquals(key,"START_ROOM")){
int f=0,r=0;
if(parseRoomToken(value,f,r)){
playerFloor=f;
playerRoom=r;
    }
}
else if(stringEquals(key,"INITIAL_ENERGY")){
if(!parseIntToken(value,number)){
return false;
  }
energy=clampInt(number,0,MAX_ENERGY);
    }
else if(stringEquals(key,"INITIAL_TIME")){
if(!parseIntToken(value,number)){
return false;
    }
time=clampInt(number,0,MAX_TIME);
}else if(stringEquals(key,"INITIAL_SCORE")){
if(!parseIntToken(value,number)){
return false;
}
score=number;
}else if(stringEquals(key,"INITIAL_INVENTORY")){
if(!stringEquals(value,"NONE")){
for(int i=0;value[i]!='\0';i++){
char c=value[i];
if(c==' '||c==','){
continue;
     }
if(!isKnownEntity(c)||inventoryCount>=MAX_INVENTORY_CAPACITY){
return false;
   }
inventory[inventoryCount]=c;
inventoryCount++;
  }
    }
}
else if(stringEquals(key,"CLUES_COLLECTED")){
if(!stringEquals(value,"NONE")){
for(int i=0;value[i]!='\0';i++){
if(value[i]=='4'){
objectives[OBJ_CLUE_4]=true;
    }else if(value[i]=='2'){
objectives[OBJ_CLUE_2]=true;
 }else if(value[i]=='7'){
objectives[OBJ_CLUE_7]=true;
              }
            }
           }
         }
       }
file.close();
return true;
}

bool validateStartingRoom(int startFloor,int startRoom,const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR-1])
{
    (void)horizontalConn;
    if(startFloor<1||startFloor>NUM_FLOORS){
        return false;
    }
    if(startRoom<1||startRoom>ROOMS_PER_FLOOR){
        return false;
    }
    return true;
}

    
    
    
    
