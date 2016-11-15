#include "ControlConsole.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <stdlib.h>
using namespace std;

ControlConsole::ControlConsole(string routeConfigFile, string idSem, string idMem){
   this->routeConfigFile = routeConfigFile;
   this->idSem = idSem;
   this->idMem = idMem;
}
ControlConsole::ControlConsole(){
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

void ControlConsole::errorFile(string token){
   cerr << "BAD FILE: " << token << endl;
   exit(1);
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

bool ControlConsole::check(bool correctArgs, bool isNumber){
   if(!(correctArgs == true && isNumber == true)){
      error();
      return false; 
   }
   
   return true;
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
   bool result;
   switch(code){
      case 0:
         iss >> command >> id;
         correctArgs = checkArguments(count, 2);
         result = check(correctArgs, true);
         if(result){
            callThread(command, id, number);
         }
         break;
      case 1:
         iss >> command >> id >> number;
         isNumber = isaNumber(number);
         correctArgs = checkArguments(count, 3);
         result = check(correctArgs, isNumber);
         if(result){
            callThread(command, id, number);
         }
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
}

void ControlConsole::callThread(string command, string id, string number){
   try {
      consoleThreadsMap.at(id)->callNotifyWrite(command, id, number);
      //this->waitNotify();
   }catch (const out_of_range& oor) {
      cerr << "Out of Range error: " << endl;
   }
}

void ControlConsole::listar(string id, string inputString){
   consoleThreadsMap.at(id)->callNotifyRead(inputString);
}

void ControlConsole::callNotify(){
   //unique_lock<mutex> lock(mut);
   this->notify = true;
   condVar.notify_all();
   cout << "notify_Out" << endl;
}

void ControlConsole::waitNotify(){
   unique_lock<mutex> lc(mut);
   cout << notify << endl;
   //while(!notify){
      //cout << notify << endl;
      //condVar.wait(lock);
   //}
      condVar.wait(lc, [&](){ return notify;});
      cout << "wait" << endl;

}

SuicideProcess* ControlConsole::getProcessInfo(string line){
   istringstream iss(line);
   int numWords = countWords(line);
   if(numWords != 8){
         errorFile("More lines");
   }

   SuicideProcess* suicideProcess = new SuicideProcess;

   string token;
   iss >> token;
   if(token.compare("ProcesoSui") != 0){
         errorFile(token);
   }

   iss >> token;
   if(token.find_first_of("0123456789") == 0){
         errorFile(token);
   }

   suicideProcess->id = token;

   iss >> token;
   if(token.compare("{") != 0){
         errorFile(token);
   }

   // get file's path
   iss >> token;

   suicideProcess->filePath = token;

   iss >> token;
   if(token.compare("::") != 0){
         errorFile(token);
   }

   // get process' name
   iss >> token;

   suicideProcess->fileName = token;

   iss >> token;
   int lives = stoi(token);
   if(lives < 0){
         errorFile(token);
   }

   suicideProcess->lives = lives;

   iss >> token;
   if(token.compare("}") != 0){
         errorFile(token);
   }

   return suicideProcess;
}

void ControlConsole::readFile(string file){
	string line;
   ifstream myfile(file, ifstream::in);
   if (myfile.is_open()){
      while (getline(myfile,line)){
         SuicideProcess* suicideProcess = getProcessInfo(line);
         consoleThreadsMap[suicideProcess->id] = new ConsoleThread(suicideProcess, idMem, idSem);
      }
      myfile.close();

      createThreads();
   }else cout << "Unable to open file" << endl;
}

void ControlConsole::createThreads(){
   for(auto it=consoleThreadsMap.begin(); it!=consoleThreadsMap.end(); ++it){
      it->second->createThread();
   }

  /* for(auto it=consoleThreadsMap.begin(); it!=consoleThreadsMap.end(); ++it){
      it->second->join();
   }*/
}


