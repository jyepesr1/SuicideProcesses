#include "ControlConsole.h"
#include<stdio.h>
#include<iostream>
#include <sstream>      // std::istringstream
#include <string> 
using namespace std;

ControlConsole::ControlConsole(string routeConfigFile, string idSem, string idMem){
   this->routeConfigFile = routeConfigFile;
   this->idSem = idSem;
   this->idMem = idMem;
}

void ControlConsole::createInterpreter(){
   string line;
   string command, id, number;
   int count = 0;
   while(cin){
      cout << "conctrl> ";
      getline(cin, line);
      istringstream iss(line);
      count = countWords(line);
      iss >> command >> id >> number; 
      cout << "command: " << command << " id: " << count << endl;
   }

}

int ControlConsole::countWords(string inputstring){
   int count(0);
   istringstream iss(inputstring);
   string word;
   while(iss >> word){
      count++;
   }
   return count;
}
