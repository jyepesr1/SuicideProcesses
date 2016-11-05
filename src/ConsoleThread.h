#ifndef THREAD_H
#define THREAD_H
#include<string>
#include "readFile.h"
using namespace std;

class ConsoleThread{
   private:
      string path, id, name;
      int lives;
   public:
      ConsoleThread(SuicideProcess* suicideProcesses);
      void createThread();
      static void *worker_thread(void * args);
      void entry();
};
#endif
