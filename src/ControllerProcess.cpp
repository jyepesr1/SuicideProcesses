#include <getopt.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "ControllerProcessAux.h"
using namespace std;


/*
void getOperation(string command, string id, string number){
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
         cout << "listar" << endl;
         break;
      default:
         cout << "nothing" << endl;
   }
}*/

int main(int argc, char *argv[]){
   int arg;
   struct option long_options[] = {
      {"filepath", required_argument, NULL, 'p'},
      {"filename", required_argument, NULL, 'n'},
      {"reencarnacion", required_argument, NULL, 'r'},
      {"memoriacompartida", required_argument, NULL, 'm'},
      {"semaforo", required_argument, NULL, 's'},
      {NULL, 0, NULL, 0}
   };
   string filepath, filename, lives, idMem, idSem;
   while((arg = getopt_long(argc, argv, "pnr:ms::", long_options, NULL)) != -1){
      switch(arg){
         case 'p':
            filepath = optarg;
            break;
         case 'n':
            filename = optarg;
            break;
         case 'r':
            lives = optarg;
            break;
         case 'm':
            idMem = optarg;
            break;
         case 's':
            idSem = optarg;
            break;
         default:
            cout << "Bad argument " << endl;
            break;
      }
   }
   
   ControllerProcessAux* controllerProcessAux = new ControllerProcessAux(filepath, filename, lives, idMem, idSem);
   
   string command, id, number;
   while(cin){
      getline(cin, command);
      getline(cin, id);
      getline(cin, number);
      controllerProcessAux->getOperation(command, id, number);
   }
}
