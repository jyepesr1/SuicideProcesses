#pragma once
#include<string>
#include<thread>
#include "suicideProcessStruct.h"
#include <condition_variable>
#include <mutex>

using namespace std;

class ConsoleThread{
   private:
      const int READ_END = 0;
      const int WRITE_END = 1;
      condition_variable cvRead, cvWrite;
      mutex mutRead, mutWrite;
      string command;
      SuicideProcess* suicideProcess;
      pid_t pid;
      int fd[3][2];
      thread consoleThreadRead;
      thread consoleThreadWrite;
      void createControllerProcess();
      void readBuffer();
      void writeBuffer();
   public:
      ConsoleThread(SuicideProcess* suicideProcesses);
      ~ConsoleThread();
      void createThread();
      void callNotifyWrite(string command);
      void callNotifyRead(string command);
      void join();
};
