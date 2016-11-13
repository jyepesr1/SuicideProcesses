#pragma once
#include <string>

using namespace std;

class ControllerProcessAux{
    private:
        string filePath, fileName, lives, idMem, idSem;
    public:
        ControllerProcessAux(string filePath, string fileName, string lives, string idMem, string idSem);
        void getOperation(string command, string id, string number);
};