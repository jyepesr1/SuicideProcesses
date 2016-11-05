#include<iostream> 
#include<string>
#include<stdlib.h>
#include "ControlConsole.h"

using namespace std;

void showUsage(){
   cerr << "Usage: \n"
        << "Options: \n"
        << "\t [--ficheroconfiguracion=<rutaficherocfg>]\n"
        << "\t [--semaforo=<id>]\n"
        << "\t [--memoriacompartida=<id>]\n"
        << endl;
   exit(1);   
}

void split(string argument, string& routeConfigFile, string& idSem, string& idMem){
   int pos = argument.find("=");
   string key = argument.substr(0,pos);
   string value = argument.substr(pos+1);
   if(key == "--ficheroconfiguracion"){
      routeConfigFile = value;
   }else if(key == "--semaforo"){
      idSem = value;
   }else if(key == "--memoriacompartida"){
      idMem = value;
   }else{
      showUsage();
   }
}

int main(int argc, const char *argv[]){
   string routeConfigFile;
   string idSem;
   string idMem;

   if(argc >= 4){
      showUsage();
   }

   for(int i=1; i<argc; i++){
      split(argv[i], routeConfigFile, idSem, idMem);
   }

   if(routeConfigFile == ""){
      routeConfigFile = "conctrl.cfg";
   }

   if(idSem == ""){
      idSem = "conctrlsem";
   }

   if(idMem == ""){
      idMem = "conctrlmem";
   }
   ControlConsole controlConsole(routeConfigFile, idSem, idMem);
   controlConsole.createInterpreter();  

   cout << "R:" << routeConfigFile << " S: " << idSem << " M: " << idMem << endl;
}

