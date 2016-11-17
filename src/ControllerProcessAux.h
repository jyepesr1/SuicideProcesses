#pragma once
#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>

using namespace std;

class ControllerProcessAux{
    private:
        thread suicide;
        bool INFINITE = false, executionStatus = true;
        mutex mutExecution;
        condition_variable executionVar;
        string filePath, fileName, idMem, idSem;
        int lives;
        pid_t pid;
        int f[2];
        int fd[3][2];
        const int READ_END = 0;
        const int WRITE_END = 1;
    public:
        ControllerProcessAux(string filePath, string fileName, string lives, string idMem, string idSem);
        void readBuffer();
        void getOperation(string command, string id, string number);
        void createSuicideProcess();
        void list();
        void sum(int num);
        void sub(int num);
        void suspend();
        void restore();
        void undefine();
        void define(int num);
        void end();
};