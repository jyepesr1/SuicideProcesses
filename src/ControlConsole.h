#pragma once

#include<string>
#include<map>
#include<thread>
#include "ConsoleThread.h"
#include "suicideProcessStruct.h"

using namespace std;

class ControlConsole{
   private:
      string routeConfigFile;
      string idSem;
      string idMem;
      map<string,ConsoleThread*> consoleThreadsMap;

   public:
      ControlConsole(string routeConfigFile, string idSem, string idMem);
      void createThreads();
      int countWords(string inputstring);
      void createInterpreter();
      void checkGrammar(string inputstring);
      bool isaNumber(string str);
      bool checkArguments(int count, int num);
      void error();
      void check(bool correctArgs, bool isaNumber);
      SuicideProcess* getProcessInfo(string line);
      void readFile(string file);
      void errorFile(string line);
};
