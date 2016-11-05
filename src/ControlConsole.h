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
      void checkGrammar(string inputstring);
      bool isaNumber(string str);
      bool checkArguments(int count, int num);
      void error();
      void check(bool correctArgs, bool isaNumber);
};
#endif
