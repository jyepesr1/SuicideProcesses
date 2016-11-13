#include "ControllerProcessAux.h"
#include <map>
#include <iostream>

ControllerProcessAux::ControllerProcessAux(string filePath, string fileName, string lives, string idMem, string idSem){
    this->filePath = filePath;
    this->fileName = fileName;
    this->lives = lives;
    this->idMem = idMem;
    this->idSem = idSem;
}

void ControllerProcessAux::getOperation(string command, string id, string number){
     int code;
   map<string, int> commands;
   commands["listar"] = 0;
   commands["sumar"] = 1;
   commands["restar"] = 2;
   commands["suspender"] = 3;
   commands["restablecer"] = 4;
   commands["indefinir"] = 5;
   commands["definir"] = 6;
   commands["terminar"] = 7;
   code = commands.find(command)->second;
   switch(code){
      case 0:
         cout << "You are in listar" << endl;
         break;
      default:
         cout << "nothing" << endl;
   }
}