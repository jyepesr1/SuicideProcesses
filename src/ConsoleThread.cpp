#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "ConsoleThread.h"
#include <string>
#include <unistd.h>

ConsoleThread::ConsoleThread(SuicideProcess* suicideProcesses){
   this->path = suicideProcesses->filePath;
   this->id = suicideProcesses->id;
   this->lives = suicideProcesses->lives;
   this->name = suicideProcesses->fileName;
}

void ConsoleThread::createThread(){
   pthread_t my_thread;
   int ret;
   printf("In main: creating thread\n");
   ret =  pthread_create(&my_thread, NULL, &ConsoleThread::worker_thread, this);
   if(ret != 0) {
      printf("Error: pthread_create() failed\n");
      exit(EXIT_FAILURE);
   }
   pthread_exit(NULL);
}

void *ConsoleThread::worker_thread(void *arg){
   ((ConsoleThread *) arg)->entry();
   return NULL;
}

void ConsoleThread::entry(){
   //pid_t son;
   //fprintf(stdout, "My PID: %d\n", getpid());
   //son = fork();
   printf("This is worker_thread()\n");
   pthread_exit(NULL);
}
/* int main(){
   ConsoleThread* c = new ConsoleThread();
   c->createThread();
 }*/
