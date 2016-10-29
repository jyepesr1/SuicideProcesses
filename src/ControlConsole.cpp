#include "ControlConsole.h"
#include<stdio.h>
using namespace std;

extern int yylex(void);
extern FILE* yyin;
ControlConsole::ControlConsole(string routeConfigFile, string idSem, string idMem){
   this->routeConfigFile = routeConfigFile;
   this->idSem = idSem;
   this->idMem = idMem;
}

void ControlConsole::CreateInterpreter(){
   string line;
   yyin = stdin;
   yylex();
}
