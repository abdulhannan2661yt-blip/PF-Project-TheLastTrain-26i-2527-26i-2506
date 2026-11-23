#include "util.h"
#include <iostream>
#include <cctype>
using namespace std;

int clampInt(int val,int minVal,int maxVal)
{
    if (val<minVal){
        return minVal;
    }
    if(val>maxVal){
        return maxVal;
    }
return val;
}
bool isSpaceChar(char c)
{
    return c==' '||c=='\t'||c=='\n'||c=='\r';
}

void trimWhitespace(char* str)
{
if(str==nullptr){
return;
}
int len=0;
while(str[len]!='\0'){
len++;
}
int start=0;
while(start<len&&isSpaceChar(str[start])){
start++;
}
if(start==len){
str[0]='\0';
return;
}
int end=len-1;
while(end>start&&isSpaceChar(str[end])){
end--;
}
int k=0;
for(int i=start;i<=end;i++){
str[k]=str[i];
k++;
}
str[k]='\0';
}
char toLowerChar(char c)
{
if(c>='A' && c<='Z'){
return c+32;
}
return c;
}

bool stringEquals(const char* a,const char* b)
{
if(a==nullptr||b==nullptr){
return false;
}
int i=0;
while(a[i]!='\0'&&b[i]!='\0'){
if(toLowerChar(a[i])!=toLowerChar(b[i])){
return false;
}
i++;
}
return a[i]=='\0'&&b[i]=='\0';
}

void printHeader(const char* title)
{cout << "\n>>>>>" << title << "<<<<<\n";
}

void clearScreen()
{
    cout<<"\033[2J\033[1;1H";
    }
    
bool parseIntToken(const char* tok,int& out){
    if (tok == nullptr || *tok=='\0')
        return false;
    const char* p = tok;
    bool negative = false;
    if(*p== '-'){
        negative = true;
        p++;}
    if(*p== '\0')
        return false;
    long value=0;
    for(; *p != '\0'; p++){
        if(*p <'0'|| *p>'9')
            return false;
        value = value*10+(*p- '0');
        if(value>100000)
            return false;
    }
    out = static_cast<int>(negative ? -value : value);
    return true;
}
bool parseRoomToken(const char* tok, int& floor, int& room) {
    if(tok== nullptr)
        return false;
if(toupper(static_cast<unsigned char>(tok[0])) !='F')
return false;
if(tok[1] < '0' || tok[1]> '9')
return false;
if (tok[2] !='-')
return false;
    if(toupper(static_cast<unsigned char>(tok[3])) != 'R')
        return false;
if (tok[4]< '0'|| tok[4] >'9')
return false;
    if (tok[5] != '\0')
        return false;
floor = tok[1] - '0';
room= tok[4] - '0';
return true;
}
int tokenizeLine(char *line,char* tokens[],int maxTokens){
    int count=0;
    char*p =line;
    while(*p != '\0' && count < maxTokens) {
        while (*p != '\0' && isspace(static_cast<unsigned char>(*p))) p++;
        if (*p == '\0') break;
        tokens[count++] = p;
        while (*p != '\0' && !isspace(static_cast<unsigned char>(*p))) p++;
        if (*p != '\0') { *p = '\0'; p++; }
    }
    return count;
}
