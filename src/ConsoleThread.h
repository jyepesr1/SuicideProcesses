#pragma once

#include<string>
#include<thread>
#include "suicideProcessStruct.h"
using namespace std;

class ConsoleThread{
   private:
      string path, id, name;
      int lives;
      thread consoleThread;
      void entry();
   public:
      ConsoleThread(SuicideProcess* suicideProcesses);
      ~ConsoleThread();
      void createThread();
      void join();
};
