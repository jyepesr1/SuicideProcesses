#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<sstream>
#include<stdlib.h>
#include<vector>
#include "ConsoleThread.h"
#include "readFile.h"

using namespace std;

int countWords(string inputString){
   int count(0);
   istringstream iss(inputString);
   string word;
   while(iss >> word){
      count++;
   }
   return count;
}

void error(string token){
   cerr << "BAD FILE: " << token << endl;
   exit(1);
}


SuicideProcess* getProcessInfo(string line){
   istringstream iss(line);
   int numWords = countWords(line);
   if(numWords != 8){
      error("More lines");
   }

   SuicideProcess* suicideProcess = new SuicideProcess;

   string token;
   iss >> token;
   if(strcmp(token.c_str(),"ProcesoSui") != 0){
      error(token);
   }

   iss >> token;
   if(token.find_first_of("0123456789") == 0){
      error(token);
   }

   suicideProcess->id = token;

   iss >> token;
   if(strcmp(token.c_str(),"{") != 0){
      error(token);
   }

   // get file's path
   iss >> token;

   suicideProcess->filePath = token;

   iss >> token;
   if(strcmp(token.c_str(),"::") != 0){
      error(token);
   }

   // get process' name
   iss >> token;

   suicideProcess->fileName = token;

   iss >> token;
   int lives = stoi(token);
   if(lives < 0){
      error(token);
   }

   suicideProcess->lives = lives;

   iss >> token;
   if(strcmp(token.c_str(),"}") != 0){
      error(token);
   }

   cout << "Exito" << endl;

   return suicideProcess;
}

int main(int argc, const char* argv[]){
   vector<ConsoleThread*> consoleThreads;
   string line;
   ifstream myfile (argv[1]);
   if (myfile.is_open()){
      while (getline(myfile,line)){
         SuicideProcess* suicideProcess = getProcessInfo(line);
         consoleThreads.push_back(new ConsoleThread(suicideProcess));
      }
      myfile.close();

      for(auto i: consoleThreads){
         i->createThread();
      }
   }else cout << "Unable to open file";

   return 0;
}



