#include <getopt.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "ControllerProcessAux.h"

using namespace std;

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
   string filepath, filename, lives;
   int idMem, idSem;
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
            idMem = stoi(optarg);
            break;
         case 's':
            idSem = stoi(optarg);
            break;
         default:
            cerr << "Error: Bad arguments. " << endl;
            break;
      }
   }
   if(argc > 12){
      cerr << "Error: Bad arguments. " << endl;
      exit(1);
   }
   int controllerNum = stoi(argv[11]);
   ControllerProcessAux* controllerProcessAux = new ControllerProcessAux(filepath, filename, lives, idMem, idSem);
   string command, id, number, temp;
   getline(cin, id);
   getline(cin, temp);
   int mapSize = stoi(temp);
   controllerProcessAux->setId(id);
   controllerProcessAux->setControllerNum(controllerNum);
   controllerProcessAux->setMapSize(mapSize);
   controllerProcessAux->initializeSharedMemory();
   controllerProcessAux->initializeSem();
   while(cin){
      getline(cin, command);
      getline(cin, id);
      getline(cin, number);
      controllerProcessAux->getOperation(command, number);
   }
   
   //delete controllerProcessAux;
}
