#include <iostream>
#include <stdlib.h>
#include "ConsoleThread.h"
#include <unistd.h>
#include <sys/types.h>
#include "ControlConsole.h"
#include <stdio.h>
#include <string.h>


ConsoleThread::ConsoleThread(SuicideProcess* suicideProcess){
   //this->path = suicideProcess->filePath;
   //this->id = suicideProcess->id;
   //this->lives = suicideProcess->lives;
   //this->name = suicideProcess->fileName;
   this->suicideProcess = suicideProcess;
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
         /*char line[1024];
         while(fgets(line, sizeof(line), stream)){
            printf("%s", line);
         }
         fclose(stream);
         cout << "line" << endl;*/
         /*
         if ((n = read(fd[0][READ_END], buf, 1)) >= 0) {
            
      	   buf[n] = 0;	 terminate the string 
      	   cout << buf;
      	}else{
      	   //cerr << "error" << endl;
      	   perror("read");
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
      strncpy(com, command.c_str(),sizeof(com));
      com[sizeof(com)-1] = 0;
      close(fd[1][READ_END]);
      FILE *stream;
      stream = fdopen(fd[1][WRITE_END], "w");
      fprintf(stream, "%s", com);
      fprintf(stream, "\n");
      fclose(stream);
      cvRead.notify_one();
   }
}

void ConsoleThread::callNotifyWrite(string command){
   unique_lock<mutex> lock(mutWrite);
   this->command = command;
   cvWrite.notify_one();
}

void ConsoleThread::callNotifyRead(string command){
   unique_lock<mutex> lockRead(mutRead);
   this->command = command;
   cvRead.notify_one();
}

void ConsoleThread::createControllerProcess(){
   cout << "Thread: " << suicideProcess->id << " Lives: " << suicideProcess->lives << endl;
   pipe(fd[0]);
   pipe(fd[1]);
   pipe(fd[2]);;
   pid = fork();
   string filePath = suicideProcess->filePath;
   string fileName = suicideProcess->fileName;
   string lives = to_string(suicideProcess->lives);
   switch(pid){
      case 0:
         close(fd[0][READ_END]);
         dup2(fd[0][WRITE_END], STDOUT_FILENO);
         close(fd[1][WRITE_END]);
         dup2(fd[1][READ_END], STDIN_FILENO);
         close(fd[2][READ_END]);
         dup2(fd[2][WRITE_END], STDERR_FILENO);
         execl("./procesoctrl", "procesoctrl", "--filepath", filePath.c_str(), 
         "--filename", fileName.c_str(), "--reencarnacion", lives.c_str(), NULL);
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

