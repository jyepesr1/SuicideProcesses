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

bool isaNumber(string str){
   return str.find_first_not_of("0123456789") == string::npos;
}

void split(string argument, string& routeConfigFile, int& idSem, int& idMem){
   int pos = argument.find("=");
   string key = argument.substr(0,pos);
   string value = argument.substr(pos+1);
   if(key == "--ficheroconfiguracion"){
      routeConfigFile = value;
   }else if(key == "--semaforo"){
      if(!isaNumber(value)){
         cerr << "Error: Wrong semaphore id, it must be a numerical id." << endl;
         exit(1);
      }
      idSem = stoi(value);
   }else if(key == "--memoriacompartida"){
      if(!isaNumber(value)){
         cerr << "Error: Wrong shared memory id,  it must be a numerical id." << endl;
         exit(1);
      }
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
   
   
   ControlConsole* controlConsole = new ControlConsole(routeConfigFile, idSem, idMem);
   controlConsole->readFile(routeConfigFile);
   controlConsole->createInterpreter();
   
   //delete controlConsole;
   
   return 0;
}

