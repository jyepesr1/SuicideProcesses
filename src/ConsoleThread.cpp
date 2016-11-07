#include <stdio.h>
#include <stdlib.h>
#include "ConsoleThread.h"
#include <unistd.h>

ConsoleThread::ConsoleThread(SuicideProcess* suicideProcesses){
   this->path = suicideProcesses->filePath;
   this->id = suicideProcesses->id;
   this->lives = suicideProcesses->lives;
   this->name = suicideProcesses->fileName;
}

void ConsoleThread::createThread(){
   printf("In main: creating thread\n");
   this->consoleThread = thread(&ConsoleThread::entry, this);
   //thread myThread(&ConsoleThread::entry, this);
}

void ConsoleThread::entry(){
   printf("This is worker_thread()\n");
}

void ConsoleThread::join(){
   this->consoleThread.join();
}

