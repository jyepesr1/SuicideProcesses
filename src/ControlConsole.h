#pragma once

#include<string>
#include<map>
#include<thread>
#include "ConsoleThread.h"
#include "suicideProcessStruct.h"
#include "MemoriaCompartida.h"
#include <condition_variable>
#include <mutex>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

using namespace std;

class ControlConsole{
   private:
      string routeConfigFile;
      int idMem, idSem, id_MemZone;
      mutex mut;
      map<string,ConsoleThread*> consoleThreadsMap;
      thread controlConsoleThreadCheckControllerProcesses;
      MemoriaCompartida *sharedMemory;
      
   public:
      condition_variable condVar;
      bool notify = false;
      ControlConsole(string routeConfigFile, int idSem, int idMem);
      ~ControlConsole();
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
      void callThread(string command, string id, string number);
      void checkControllerProcesses();
      void createSharedMemory();
      void readSharedMemory();
};
