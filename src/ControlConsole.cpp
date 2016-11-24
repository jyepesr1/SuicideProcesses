#include "ControlConsole.h"

using namespace std;

int controllerProcessesAlive;

ControlConsole::ControlConsole(string routeConfigFile, int idSem, int idMem){
   this->routeConfigFile = routeConfigFile;
   this->idSem = idSem;
   this->idMem = idMem;
}

void ControlConsole::createInterpreter(){
   string line;
   while(cin){
      cout << "conctrl> ";
      getline(cin, line);
      if(line.length() == 0){
         exit(EXIT_SUCCESS);
      }
      checkGrammar(line);
   }

}

int ControlConsole::countWords(string inputString){
   int count(0);
   istringstream iss(inputString);
   string word;
   while(iss >> word){
      count++;
   }
   return count;
}

bool ControlConsole::isaNumber(string str){
   return str.find_first_not_of("0123456789") == string::npos;
}

bool ControlConsole::checkArguments(int count, int numArgs){
   return count == numArgs;
}

void ControlConsole::errorFile(string token){
   cerr << "BAD FILE: " << token << endl;
   exit(1);
}

void ControlConsole::error(){
   cerr << "Usage: \n"
        << "\tlistar IdProcCtrl \n"
        << "\tsumar IdProcCtrl NUMBER \n"
        << "\tsuspender IdProcCtrl\n"
        << "\trestablecer IdProcCtrl \n"
        << "\tindefinir IdProcCtrl \n"
        << "\tdefinir IdProcCtrl NUMBER\n"
        << "\tterminar IdProcCtrl"
        << endl;
}

bool ControlConsole::check(bool correctArgs, bool isNumber){
   if(!(correctArgs == true && isNumber == true)){
      error();
      return false; 
   }
   
   return true;
}

void ControlConsole::checkGrammar(string inputString){
   map<string, int> commands;
   string command, id, number = "";
   int count = 0, code = -1;
   commands["listar"] = 0;
   commands["sumar"] = 1;
   commands["restar"] = 2;
   commands["suspender"] = 3;
   commands["restablecer"] = 4;
   commands["indefinir"] = 5;
   commands["definir"] = 6;
   commands["terminar"] = 7;
   istringstream iss(inputString);
   istringstream iss1(inputString);
   iss1 >> command;
   code = commands.find(command)->second;
   count = countWords(inputString);
   bool isNumber = false, correctArgs = false;
   bool result = false;
   switch(code){
      case 0:
         iss >> command >> id;
         correctArgs = checkArguments(count, 2);
         result = check(correctArgs, true);
         break;
      case 1:
         iss >> command >> id >> number;
         isNumber = isaNumber(number);
         correctArgs = checkArguments(count, 3);
         result = check(correctArgs, isNumber);
         break;
      case 2:
         iss >> command >> id >> number;
         isNumber = isaNumber(number);
         correctArgs = checkArguments(count, 3);
         result = check(correctArgs, isNumber);
         break;
      case 3:
         iss >> command >> id;
         correctArgs = checkArguments(count, 2);
         result = check(correctArgs, true);
         break;
      case 4:
         iss >> command >> id;
         correctArgs = checkArguments(count, 2);
         result = check(correctArgs, true);
         break;
      case 5:
         iss >> command >> id;
         correctArgs = checkArguments(count, 2);
         result = check(correctArgs, true);
         break;
      case 6:
         iss >> command >> id >> number;
         isNumber = isaNumber(number);
         correctArgs = checkArguments(count, 3);
         result = check(correctArgs, isNumber);
         break;
      case 7:
         iss >> command >> id;
         correctArgs = checkArguments(count, 2);
         result = check(correctArgs, true);
         break;
      default:
         error();
         break;
   }
   if(result){
      callThread(command, id, number);
      //this->waitNotify();
   }
}

void ControlConsole::callThread(string command, string id, string number){
   if(id == "*"){
      try{
         for(auto it=consoleThreadsMap.begin(); it!=consoleThreadsMap.end(); ++it){
            //cout << it->first;
            it->second->callNotifyWrite(command,it->first, number);
            if(command == "terminar") exit(1);
         }
      }catch(const out_of_range& oor){
         cerr << "Out of Range error" << endl;
      }
   }else{
      try {
         consoleThreadsMap.at(id)->callNotifyWrite(command, id, number);
         if(command == "terminar") consoleThreadsMap.erase(id);
      }catch (const out_of_range& oor) {
         cerr << "Out of Range error: " << endl;
      }  
   }
}

void ControlConsole::listar(string id, string inputString){
   consoleThreadsMap.at(id)->callNotifyRead(inputString);
}

void ControlConsole::callNotify(){
   //unique_lock<mutex> lock(mut);
   this->notify = true;
   condVar.notify_all();
   cout << "notify_Out" << endl;
}

void ControlConsole::waitNotify(){
   unique_lock<mutex> lock(mut);
   while(!notify){
      cout << notify << endl;
      condVar.wait(lock);
   }
   //condVar.wait(lc, [&](){ return notify;});
   cout << "wait" << endl;

}

SuicideProcess* ControlConsole::getProcessInfo(string line){
   istringstream iss(line);
   int numWords = countWords(line);
   if(numWords != 8){
         errorFile("More lines");
   }

   SuicideProcess* suicideProcess = new SuicideProcess;

   string token;
   iss >> token;
   if(token.compare("ProcesoSui") != 0){
         errorFile(token);
   }

   iss >> token;
   if(token.find_first_of("0123456789") == 0){
         errorFile(token);
   }

   suicideProcess->id = token;

   iss >> token;
   if(token.compare("{") != 0){
         errorFile(token);
   }

   // get file's path
   iss >> token;

   suicideProcess->filePath = token;

   iss >> token;
   if(token.compare("::") != 0){
         errorFile(token);
   }

   // get process' name
   iss >> token;

   suicideProcess->fileName = token;

   iss >> token;
   int lives = stoi(token);
   if(lives < 0){
         errorFile(token);
   }

   suicideProcess->lives = lives;

   iss >> token;
   if(token.compare("}") != 0){
         errorFile(token);
   }

   return suicideProcess;
}

void ControlConsole::readFile(string file){
	string line;
   ifstream myfile(file, ifstream::in);
   if (myfile.is_open()){
      while (getline(myfile,line)){
         SuicideProcess* suicideProcess = getProcessInfo(line);
         consoleThreadsMap[suicideProcess->id] = new ConsoleThread(suicideProcess, idMem, idSem);
      }
      myfile.close();

      controlConsoleThreadCheckControllerProcesses = thread(&ControlConsole::checkControllerProcesses, this);
      createThreads();
   }else cout << "Unable to open file" << endl;
}

void ControlConsole::createThreads(){
   for(auto& thread : consoleThreadsMap){
      thread.second->createThread();
   }
   
   /*for(auto it=consoleThreadsMap.begin(); it!=consoleThreadsMap.end(); ++it){
      it->second->createThread();
   }*/

  /* for(auto it=consoleThreadsMap.begin(); it!=consoleThreadsMap.end(); ++it){
      it->second->join();
   }*/
   createSharedMemory();
}

void ControlConsole::checkControllerProcesses(){
   controllerProcessesAlive = consoleThreadsMap.size();
   while(controllerProcessesAlive != 0);
   exit(0);
}

void ControlConsole::createSharedMemory(){
   int key = ftok("../examples/shm", idMem);
   if (key == -1) {
      cerr << "Error with key \n" << endl;
      exit(1);
   }   
   
   int id_MemZone = shmget(key, 1024, 0666 | IPC_CREAT);
   if (id_MemZone == -1) {
      fprintf (stderr, "Error with id_MemZone 1 \n");
      exit(1); 
   }
   
   MemoriaCompartida *sharedMemory; /* shared sharedMemory */
   /* we declared to zone to share */
   sharedMemory = (MemoriaCompartida *)shmat (id_MemZone, (char *)0, 0);
   if (sharedMemory == NULL) {
      fprintf (stderr, "Error reserve shared memory \n");
      exit(1); 
   }
   
   sharedMemory->valSeq = 0;
   sharedMemory->muertes = (InfoMuerte* )(sharedMemory + 1);
   
   int i=0;
   for(auto& process : consoleThreadsMap){
      char id[256];
      string idAux = process.first;
      strncpy(id, idAux.c_str(), sizeof(id));
      strncpy(sharedMemory->muertes[i].id, id, sizeof(sharedMemory->muertes[i].id));
      i++;
   }
   
  /*sharedMemory->muertes[0].nDecesos = 3;
   strcpy(sharedMemory->muertes[0].id, "hola");
   sharedMemory->muertes[1].nDecesos = 90;
   strcpy(sharedMemory->muertes[1].id, "chao");
   */
   
   /* Free the shared memory */
}



