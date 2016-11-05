#include "ControlConsole.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string> 
#include <map>
 #include <stdlib.h>
using namespace std;

ControlConsole::ControlConsole(string routeConfigFile, string idSem, string idMem){
   this->routeConfigFile = routeConfigFile;
   this->idSem = idSem;
   this->idMem = idMem;
}

void ControlConsole::createInterpreter(){
   string line;
   while(cin){
      cout << "conctrl> ";
      getline(cin, line);
      checkGrammar(line);
   }

}

int ControlConsole::countWords(string inputString){
   int count(0);
   istringstream iss(inputString);
   string word;
   while(iss >> word){
      count++;
   }
   return count;
}

bool ControlConsole::isaNumber(string str){
   return str.find_first_not_of("0123456789") == string::npos;
}

bool ControlConsole::checkArguments(int count, int numArgs){
   return count == numArgs;
}

void ControlConsole::error(){
   cerr << "Usage: \n" 
        << "\tlistar IdProcCtrl \n"
        << "\tsumar IdProcCtrl NUMBER \n"
        << "\tsuspender IdProcCtrl\n"
        << "\trestablecer IdProcCtrl \n"
        << "\tindefinir IdProcCtrl \n"
        << "\tdefinir IdProcCtrl NUMBER\n"
        << "\tterminar IdProcCtrl"
        << endl;
}

void ControlConsole::check(bool correctArgs, bool isNumber){
   if(!(correctArgs == true && isNumber == true)){
      error();
   }
}

void ControlConsole::checkGrammar(string inputString){
   map<string, int> commands; 
   string command, id, number;
   int count = 0, code = -1;
   commands["listar"] = 0; 
   commands["sumar"] = 1;
   commands["restar"] = 2;
   commands["suspender"] = 3;
   commands["restablecer"] = 4;
   commands["indefinir"] = 5;
   commands["definir"] = 6;
   commands["terminar"] = 7;
   istringstream iss(inputString);
   istringstream iss1(inputString);
   iss1 >> command;
   code = commands.find(command)->second; 
   count = countWords(inputString);
   bool isNumber = false, correctArgs = false;
   switch(code){
      case 0:
         iss >> command >> id;
         correctArgs = checkArguments(count, 2); 
         check(correctArgs, true);
         break;
      case 1:
         iss >> command >> id >> number; 
         isNumber = isaNumber(number);
         correctArgs = checkArguments(count, 3); 
         check(correctArgs, isNumber);
         break;
      case 2:
         iss >> command >> id >> number; 
         isNumber = isaNumber(number);
         correctArgs = checkArguments(count, 3); 
         check(correctArgs, isNumber);
         break;
      case 3:
         iss >> command >> id; 
         correctArgs = checkArguments(count, 2); 
         check(correctArgs, true);
         break;
      case 4:
         iss >> command >> id; 
         correctArgs = checkArguments(count, 2); 
         check(correctArgs, true);
         break;
      case 5:
         iss >> command >> id; 
         correctArgs = checkArguments(count, 2); 
         check(correctArgs, true);
         break;
      case 6:
         iss >> command >> id >> number; 
         isNumber = isaNumber(number);
         correctArgs = checkArguments(count, 3); 
         check(correctArgs, isNumber);
         break;
      case 7:
         iss >> command >> id; 
         correctArgs = checkArguments(count, 2); 
         check(correctArgs, true);
         break;
      default:
         error();
         break;
   }
   cout << "command: " << command << " id: " << id << " number: " << number << endl;
}








