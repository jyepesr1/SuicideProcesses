#pragma once

#include<string>
#include<map>
#include<thread>
#include "ConsoleThread.h"
#include "suicideProcessStruct.h"
#include <condition_variable>
#include <mutex>

using namespace std;

class ControlConsole{
   private:
      string routeConfigFile;
      string idSem;
      string idMem;
      mutex mut;
      map<string,ConsoleThread*> consoleThreadsMap;
      

   public:
      condition_variable condVar;
      bool notify = false;
      ControlConsole(string routeConfigFile, string idSem, string idMem);
      ControlConsole();
      void createThreads();
      int countWords(string inputstring);
      void createInterpreter();
      void checkGrammar(string inputstring);
      bool isaNumber(string str);
      bool checkArguments(int count, int num);
      void error();
      bool check(bool correctArgs, bool isaNumber);
      SuicideProcess* getProcessInfo(string line);
      void readFile(string file);
      void errorFile(string line);
      void callNotify();
      void listar(string id, string inputstring);
      void waitNotify();
};
