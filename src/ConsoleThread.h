#pragma once

#include<string>
#include<thread>
#include "suicideProcessStruct.h"
using namespace std;

class ConsoleThread{
   private:
      SuicideProcess* suicideProcess;
      //string path, id, name;
      //int lives;
      thread consoleThread;
      void entry();
   public:
      ConsoleThread(SuicideProcess* suicideProcesses);
      ~ConsoleThread();
      void createThread();
      void join();
};
