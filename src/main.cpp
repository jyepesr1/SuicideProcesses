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

void split(string argument, string& routeConfigFile, int& idSem, int& idMem){
   int pos = argument.find("=");
   string key = argument.substr(0,pos);
   string value = argument.substr(pos+1);
   if(key == "--ficheroconfiguracion"){
      routeConfigFile = value;
   }else if(key == "--semaforo"){
      idSem = stoi(value);
   }else if(key == "--memoriacompartida"){
      idMem = stoi(value);
   }else{
      showUsage();
   }
}

int main(int argc, const char *argv[]){
   string routeConfigFile;
   int idMem, idSem;

   if(argc > 4){
      showUsage();
   }

   for(int i=1; i<argc; i++){
      split(argv[i], routeConfigFile, idSem, idMem);
   }

   if(routeConfigFile == ""){
      routeConfigFile = "conctrl.cfg";
   }

   if(to_string(idSem) == ""){
      string line;
      ifstream myfile ("../examples/conctrlsem");
      if (myfile.is_open()){
         while (getline(myfile,line)){
            idMem = stoi(line);
         }
         myfile.close();
      }else cout << "Unable to open file"; 
   }

   if(to_string(idMem) == ""){
      string line;
      ifstream myfile ("../examples/conctrlmem");
      if (myfile.is_open()){
         while (getline(myfile,line)){
            idMem = stoi(line);
         }
         myfile.close();
      }else cout << "Unable to open file"; 
   }
   
   ControlConsole controlConsole(routeConfigFile, idSem, idMem);
   controlConsole.readFile(routeConfigFile);
   controlConsole.createInterpreter();

   cout << "R:" << routeConfigFile << " S: " << idSem << " M: " << idMem << endl;
}

