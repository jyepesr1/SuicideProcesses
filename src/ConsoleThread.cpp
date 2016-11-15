#include <iostream>
#include <stdlib.h>
#include "ConsoleThread.h"
#include <unistd.h>
#include <sys/types.h>
#include "ControlConsole.h"
#include <stdio.h>
#include <string.h>


ConsoleThread::ConsoleThread(SuicideProcess* suicideProcess, string idMem, string idSem){
   //this->path = suicideProcess->filePath;
   //this->id = suicideProcess->id;
   //this->lives = suicideProcess->lives;
   //this->name = suicideProcess->fileName;
   this->suicideProcess = suicideProcess;
   this->idMem = idMem;
   this->idSem = idSem;
   ConsoleThread::createControllerProcess();
   
}

void ConsoleThread::createThread(){
   cout << "In main: creating thread" << endl;
   consoleThreadRead = thread(&ConsoleThread::readBuffer, this);
   consoleThreadWrite = thread(&ConsoleThread::writeBuffer, this);
}

void ConsoleThread::readBuffer(){
   bool isReading =true;
   while(isReading){
      unique_lock<mutex> lockRead(mutRead);
      bool read1 = true;
      while(read1){
         cvRead.wait(lockRead);
         
         close(fd[0][WRITE_END]);
         FILE *stream;
         int c;
         stream = fdopen (fd[0][READ_END], "r");
         while((c = fgetc(stream)) != EOF){
            putchar(c);
         }
         fclose(stream);
         //char line[1024];
         /*while(!feof(stream)){
            if(fgets(line, sizeof(line), stream) == NULL) break;
            fputs(line, stdout);
         }
         
         while(fgets(line, sizeof(line), stream)){
            printf("%s\n", line);
            fflush(stdout);
         }*/
      }
      
      if(suicideProcess->lives == 0){
         isReading = false;
      }
   }
}

void ConsoleThread::writeBuffer(){
   unique_lock<mutex> lockWrite(mutWrite);
   bool write1 = true;
   while(write1){
      cvWrite.wait(lockWrite);
      char com[1024];
      char idP[1024];
      char num[1024];
      strncpy(com, command.c_str(),sizeof(com));
      strncpy(idP, id.c_str(),sizeof(idP));
      strncpy(num, number.c_str(),sizeof(num));
      com[sizeof(com)-1] = 0;
      idP[sizeof(idP)-1] = 0;
      num[sizeof(num)-1] = 0;
      close(fd[1][READ_END]);
      FILE *stream;
      stream = fdopen(fd[1][WRITE_END], "w");
      fprintf(stream, "%s\n", com);
      fprintf(stream, "%s\n", idP);
      fprintf(stream, "%s\n", num);
      fclose(stream);
      cvRead.notify_one();
   }
   
}

void ConsoleThread::callNotifyWrite(string command, string id, string number){
   unique_lock<mutex> lock(mutWrite);
   this->command = command;
   this->id = id;
   this->number = number;
   cvWrite.notify_one();
}

void ConsoleThread::callNotifyRead(string command){
   unique_lock<mutex> lockRead(mutRead);
   this->command = command;
   cvRead.notify_one();
}

void ConsoleThread::createControllerProcess(){
   cout << "Thread: " << suicideProcess->id << " Lives: " << suicideProcess->lives << endl;
   string filePath = suicideProcess->filePath;
   string fileName = suicideProcess->fileName;
   string lives = to_string(suicideProcess->lives);
   pipe(fd[0]);
   pipe(fd[1]);
   pipe(fd[2]);
   pid = fork();
   switch(pid){
      case 0:
         
         close(fd[0][READ_END]);
         dup2(fd[0][WRITE_END], STDOUT_FILENO);
         close(fd[1][WRITE_END]);
         dup2(fd[1][READ_END], STDIN_FILENO);
         close(fd[2][READ_END]);
         dup2(fd[2][WRITE_END], STDERR_FILENO);
         
         
         /*close(fdin[WRITE_END]);
         dup2(fdin[READ_END], READ_END);
         close(fdout[READ_END]);
         dup2(fdout[WRITE_END], WRITE_END);
         cout << "into" << endl;
         close(fd[1][WRITE_END]);
         dup2(fd[1][READ_END], READ_END);
         close(fd[0][READ_END]);
         dup2(fd[0][WRITE_END], WRITE_END);
         cout << "into" << endl;
         close(fd[1][READ_END]);
         close(fd[2][READ_END]);
         dup2(fd[2][WRITE_END], STDERR_FILENO);
         close(fd[2][WRITE_END]);*/
         execl("./procesoctrl", "procesoctrl", "--filepath", filePath.c_str(), 
         "--filename", fileName.c_str(), "--reencarnacion", lives.c_str(),
         "--memoriacompartida", idMem.c_str(), "--semaforo", idSem.c_str(), NULL);
         break;
      case -1:
         cerr << "Error" << endl;
         break;
      default:
         break;
         
   }
   
}

void ConsoleThread::join(){
   consoleThreadRead.join();
   consoleThreadWrite.join();
}

