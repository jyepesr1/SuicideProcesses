#ifndef CONSOLE_H
#define CONSOLE_H
#include<string>

using namespace std;

class ControlConsole{
   private:
      string routeConfigFile;
      string idSem;
      string idMem;

   public:
      ControlConsole(string routeConfigFile, string idSem, string idMem);
      void createThreads();
      int countWords(string inputstring);
      void createInterpreter();
};
#endif
