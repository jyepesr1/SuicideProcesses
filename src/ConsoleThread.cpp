#include <iostream>
#include <stdlib.h>
#include "ConsoleThread.h"
#include <unistd.h>

ConsoleThread::ConsoleThread(SuicideProcess* suicideProcess){
   this->path = suicideProcess->filePath;
   this->id = suicideProcess->id;
   this->lives = suicideProcess->lives;
   this->name = suicideProcess->fileName;
}

void ConsoleThread::createThread(){
   cout << "In main: creating thread" << endl;
   consoleThread = thread(&ConsoleThread::entry, this);
   //thread myThread(&ConsoleThread::entry, this);
}

void ConsoleThread::entry(){
   cout << "Thread: " << id << " Lives: " << lives << endl;
}

void ConsoleThread::join(){
   consoleThread.join();
}

