#include "ConsoleThread.h"

extern int controllerProcessesAlive;

ConsoleThread::ConsoleThread(SuicideProcess* suicideProcess, int idMem, int idSem, int controllerNum){
   //this->path = suicideProcess->filePath;
   //this->id = suicideProcess->id;
   //this->lives = suicideProcess->lives;
   //this->name = suicideProcess->fileName;
   this->suicideProcess = suicideProcess;
   this->idMem = idMem;
   this->idSem = idSem;
   this->procId = suicideProcess->id;
   this->controllerNum = controllerNum;
   ConsoleThread::createControllerProcess();
   
}

void ConsoleThread::setMapSize(int mapSize){
   this->mapSize = mapSize;
   dprintf(fd[1][WRITE_END], "%i\n", mapSize);
}

void ConsoleThread::createThread(){
   consoleThreadRead = thread(&ConsoleThread::readBuffer, this);
   consoleThreadWrite = thread(&ConsoleThread::writeBuffer, this);
   consoleThreadWaitControllerProcessDeath = thread(&ConsoleThread::waitDeath, this);
}

void ConsoleThread::readBuffer(){
      //unique_lock<mutex> lockRead(mutRead);
      char buf[1025] = {0};
      //cvRead.wait(lockRead);
      close(fd[0][WRITE_END]);
      FILE *stream;
      stream = fdopen (fd[0][READ_END], "r");
      while(fgets(buf, sizeof(buf), stream)){
         cout << "conctrl> ";
         printf("%s", buf);
         fflush(stream);
      }
      fclose(stream);
}

void ConsoleThread::writeBuffer(){
   unique_lock<mutex> lockWrite(mutWrite);
   bool write1 = true;
   FILE *stream;
   stream = fdopen(fd[1][WRITE_END], "w");
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
      //FILE *stream;
      //stream = fdopen(fd[1][WRITE_END], "w");
      fprintf(stream, "%s\n", com);
      fprintf(stream, "%s\n", idP);
      fprintf(stream, "%s\n", num);
      fflush(stream);
      //fclose(stream);
      //cvRead.notify_one();
   }
   fclose(stream);
   
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
   //cout << "Thread: " << suicideProcess->id << " Lives: " << suicideProcess->lives << endl;
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
         dup2(fd[0][WRITE_END], WRITE_END);
         
         close(fd[2][READ_END]);
         dup2(fd[2][ERR_END], ERR_END);
         
         close(fd[1][WRITE_END]);
         dup2(fd[1][READ_END], READ_END);
         
         execl("./procesoctrl", "procesoctrl", "--filepath", filePath.c_str(), 
         "--filename", fileName.c_str(), "--reencarnacion", lives.c_str(),
         "--memoriacompartida", to_string(idMem).c_str(), "--semaforo", to_string(idSem).c_str(),
         to_string(controllerNum).c_str(), NULL);
         cerr << "Error creating Process Controller" << endl;
         break;
      case -1:
         cerr << "Error creating Process Controller" << endl;
         break;
      default:
         dprintf(fd[1][WRITE_END], "%s\n", procId.c_str());
         //write(fd[1][WRITE_END], procId + "", 5);
         break;
         
   }
   
}

void ConsoleThread::joinThreads(){
   consoleThreadRead.join();
   consoleThreadWrite.join();
   consoleThreadWaitControllerProcessDeath.join();
}

void ConsoleThread::waitDeath(){
   int childStatus;
   waitpid(pid, &childStatus, 0);
   controllerProcessesAlive--;
   joinThreads();
}

ConsoleThread::~ConsoleThread(){
   delete suicideProcess;
}