#pragma once

#include <string>
#include <string.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <map>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "MemoriaCompartida.h"


using namespace std;

class ControllerProcessAux{
    private:
        thread suicide, readthread;
        int mapSize = 0;
        bool INFINITE = false, executionStatus = true;
        mutex mutExecution;
        mutex mut;
        condition_variable executionVar;
        string filePath, fileName;
        int idMem, idSem, id_MemZone, lives, sem_id, controllerNum;
        pid_t pid;
        int fd[3][3];
        MemoriaCompartida *sharedMemory;
        const int READ_END = 0;
        const int WRITE_END = 1;
        const int ERR_END = 2;
        const size_t MAX = 2048;
        string id;
        void sem_lock();
        void sem_unlock();
    public:
        ControllerProcessAux(string filePath, string fileName, string lives, int idMem, int idSem);
        ~ControllerProcessAux();
        void readBuffer();
        void getOperation(string command, string number);
        void createSuicideProcess();
        void list();
        void sum(int num);
        void sub(int num);
        void suspend();
        void restore();
        void undefine();
        void define(int num);
        void end();
        void initializeSharedMemory();
        void writeSharedMemory();
        void initializeSem();
        void setId(string id);
        void setMapSize(int mapSize);
        void setControllerNum(int controllerNum);
};