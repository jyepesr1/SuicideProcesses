#pragma once

#include<string>
#include<thread>
#include "suicideProcessStruct.h"
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <fstream>

using namespace std;

class ConsoleThread{
   private:
      const int READ_END = 0;
      const int WRITE_END = 1;
      const int ERR_END = 2;
      condition_variable cvRead, cvWrite;
      mutex mutRead, mutWrite;
      string command, id, number, procId;
      int idMem, idSem, mapSize, controllerNum;
      SuicideProcess* suicideProcess;
      pid_t pid;
      int fd[3][3];
      thread consoleThreadRead;
      thread consoleThreadWrite;
      thread consoleThreadWaitControllerProcessDeath;
      void createControllerProcess();
      void readBuffer();
      void writeBuffer();
      void waitDeath();
      void joinThreads();
      
   public:
      ConsoleThread(SuicideProcess* suicideProcesses, int idMem, int idSem, int controllerNum);
      ~ConsoleThread();
      void createThread();
      void callNotifyWrite(string command, string id, string number);
      void callNotifyRead(string command);
      void setMapSize(int mapSize);
};
