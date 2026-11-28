#include "logger.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include "util.h"
#include "level.h"
using namespace std;

const int MAX_AUDIT=200;
const int ACTION_LEN=40;
static char auditAction[MAX_AUDIT][ACTION_LEN];
static int auditEnergy[MAX_AUDIT];
static int auditTime[MAX_AUDIT];
static int auditScore[MAX_AUDIT];
static int auditCount=0;
static int auditSkipped=0;

void copyText(char dest[],const char* src,int maxLen)
{
    int i=0;
    if(src!=nullptr)
    {
        while(src[i]!='\0'&&i<maxLen-1)
        {
            dest[i]=src[i];
            i++;
        }
    }
    dest[i]='\0';
}
void logAuditEntry(const char* action,int energy,int time,int score)
{
    if(auditCount>=MAX_AUDIT)
    {
        auditSkipped++;
        return ;
    }
    copyText(auditAction[auditCount],action,ACTION_LEN);
    auditEnergy[auditCount]=energy;
    auditTime[auditCount]=time;
    auditScore[auditCount]=score;
    auditCount++;
}
void writeSymbols(ofstream& out,const char s[],int count)
{
    if(count==0){
        out<<"NONE";
        
    }
    for(int i=0;i<count;i++){
        out<<s[i];
    }
    out<<endl;
}

bool saveGameStateToFile(const char* filename,int playerFloor,int playerRoom,int energy,int time,int score,const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,const bool objectives[NUM_OBJECTIVES],int emergencyTripsLeft,const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    ofstream out(filename);
    if(!out.is_open()){
        return false;
    }
    out<<"PLAYER_FLOOR: "<<playerFloor<<endl;
    out<<"PLAYER_ROOM: "<<playerRoom<<endl;
    out<<"ENERGY: "<<energy<<endl;
    out<<"TIME: "<<time<<endl;
    out<<"SCORE: "<<score<<endl;
    out<<"EMERGENCY_TRIPS: "<<emergencyTripsLeft<<endl;
    
    out<<"INVENTORY: ";
    writeSymbols(out,inventory,inventoryCount);
    
    out<<"OBJECTIVES: ";
    for(int i=0;i<NUM_OBJECTIVES;i++){
        out<<objectives[i];
    }
    out<<endl;
    for(int f=0;f<NUM_FLOORS;f++){
        for(int r=0;r<ROOMS_PER_FLOOR;r++){
            out<<"ROOM_"<<f+1<<"_"<<r+1<<": ";
            writeSymbols(out,roomEntities[f][r],entityCounts[f][r]);
        }
    }
    return !out.fail();
}

bool readSymbols(const char* value,char dest[],int maxCount,int& count)
{
    count=0;

    if(stringEquals(value,"NONE")){
        return true;
    }
    for(int i=0;value[i]!='\0';i++){
        if(count>=maxCount){
            return false;
        }
        dest[count]=value[i];
        count++;
    }
    return true;

}
bool readRoomKey(const char* key,int& f,int& r)
{
    if(key[0]!='R'||key[4]!='_'||key[6]!='_'||key[8]!='\0'){
        return false;
    }
    f=key[5]-'0';
    r=key[7]-'0';
    return f>=1&&f<=NUM_FLOORS&&r>=1&&r<=ROOMS_PER_FLOOR;
}

bool loadGameStateFromFile(const char* filename,int& playerFloor,int& playerRoom,int& energy,int& time,int& score,char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES],int& emergencyTripsLeft,char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    ifstream in(filename);
    if(!in.is_open()){
        return false;
    }
    int f=0,r=0,e =0,t= 0, s=0,trips=0,invCount=0;
    char inv[MAX_INVENTORY_CAPACITY];
    bool obj[NUM_OBJECTIVES]={};
    char ents[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM]={};
    int counts[NUM_FLOORS][ROOMS_PER_FLOOR]={};

    char line[MAX_LINE_LEN];
    while(in.getline(line,MAX_LINE_LEN)){
        char* key=nullptr;
        char* value=nullptr;
        if(!splitKeyValue(line,key,value)){
            continue;
        }
        bool ok=true;
        int rf=0,rr=0;
        if(stringEquals(key,"PLAYER_FLOOR")){
            ok=parseIntToken(value,f);
        }
        else if(stringEquals(key,"PLAYER_ROOM"))
        {
            ok=parseIntToken(value,r);
        }
        else if(stringEquals(key,"ENERGY"))
        {
            ok=parseIntToken(value,e);
        }
        else if(stringEquals(key,"TIME"))
        {
            ok=parseIntToken(value,t);
        }
        else if(stringEquals(key,"SCORE"))
        {
            ok=parseIntToken(value,s);
        }
        else if(stringEquals(key,"EMERGENCY_TRIPS"))
        {
            ok=parseIntToken(value,trips);
        }
        else if(stringEquals(key,"INVENTORY"))
        {
            ok=readSymbols(value,inv,MAX_INVENTORY_CAPACITY,invCount);
        }else if(stringEquals(key,"OBJECTIVES")){
            for(int i=0;i<NUM_OBJECTIVES&&value[i]!='\0';i++){
                obj[i]=(value[i]=='1');
            }
        }else if(readRoomKey(key,rf,rr)){
            ok=readSymbols(value,ents[rf-1][rr-1],MAX_ENTITIES_PER_ROOM,counts[rf-1][rr-1]);
        }
        if(!ok){
            cout<<"Save file error at: "<<key<<endl;
            return false;
        }
    }
    
    if(f<1||f>NUM_FLOORS||r<1||r>ROOMS_PER_FLOOR){
        cout<<"Save file error: bad player room"<<endl;
        return false;
    }
    
    playerFloor=f;
    playerRoom=r;
    energy=e;
    time=t;
    score=s;
    emergencyTripsLeft=trips;
    inventoryCount=invCount;
    for(int i=0;i<invCount;i++)
    {
    inventory[i]=inv[i];
    }
    for(int i=0;i<NUM_OBJECTIVES;i++){
        objectives[i]=obj[i];
    }
    for(int a=0;a<NUM_FLOORS; a++){
        for(int b=0;b<ROOMS_PER_FLOOR; b++){
            entityCounts[a][b]=counts[a][b];
            for(int c=0;c<MAX_ENTITIES_PER_ROOM;c++){
                roomEntities[a][b][c]=ents[a][b][c];
            }
        }
    }
    return true;
}

void writeGameSummaryReport(const char* filename,int score,int remainingEnergy,int remainingTime,const bool               objectives[NUM_OBJECTIVES],bool victory){
      ofstream out(filename);
         if(!out.is_open()){
           cout << "Error: cannot write" << filename<< endl;
               return;
       }
      out << ">>>>> THE LAST TRAIN: GAME SUMMARY<<<<<"  <<endl;
    if(victory)
      {
        out << "Result: Victory" << endl;
      }
          else if(objectives[OBJ_MISSION_FAILED]){
        out << "Result : Defeat" << endl;
    }
       else{
          out<<"Result: INCOMPLETE (player quit)"<<endl;
    }
    out<<"Final Score: "<<score<<" / 520"<<endl;
    out<<"Energy left: "<<remainingEnergy<<endl;
    out<<"Time left: "<<remainingTime<<endl;
    
    const int SHOWN=11;
       const char* names[SHOWN]={"Master Key collected","Clue 4 found","Clue 2 found","Clue 7 found","Cabinet unlocked","Train Key taken","Fuse installed","Lift Pass collected","Passenger rescued","Cat secured","Train prepared and boarded"};
    int index     [SHOWN]={OBJ_MASTER_KEY,OBJ_CLUE_4,OBJ_CLUE_2,OBJ_CLUE_7,OBJ_CABINET_UNLOCKED,OBJ_TRAIN_KEY,OBJ_FUSE_INSTALLED,OBJ_LIFT_PASS,OBJ_PASSENGER_RESCUED,OBJ_CAT_SECURED,OBJ_TRAIN_PREPARED};
    out<<endl<<"Objectives:"<<endl;
    for(int i=0;i<SHOWN;i++){
        if(objectives[index[i]]){
            out<<"[X] ";
        }else{
            out<<"[ ] ";
           }
             out<<names[i]<<endl;
    }
        out<<endl<<"Session audit ("<<auditCount<<" actions):"<<endl;
        for(int i=0;i<auditCount;i++){
          out<<i+1<<". "<<auditAction[i]<<" | E="<<auditEnergy[i]<<" T="<<auditTime[i]<<" S="<<auditScore[i]<<endl;
    }
       if(auditSkipped>0){
           out<<"("<<auditSkipped<<" more actions not recorded)"<<endl;
    }
}
