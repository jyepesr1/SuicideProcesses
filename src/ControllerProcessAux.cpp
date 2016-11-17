#include "ControllerProcessAux.h"
#include <map>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>

ControllerProcessAux::ControllerProcessAux(string filePath, string fileName, string lives, string idMem, string idSem){
    this->filePath = filePath;
    this->fileName = fileName;
    this->lives = stoi(lives);
    this->idMem = idMem;
    this->idSem = idSem;
    if(this->lives == 0){
        INFINITE = true;
    }
    suicide = thread(&ControllerProcessAux::createSuicideProcess, this);
    //suicide.join();
    //createSuicideProcess();
}

void ControllerProcessAux::readBuffer(){
    char readbuffer[1024];
    close(fd[0][WRITE_END]);
    read(fd[0][READ_END], readbuffer, sizeof(readbuffer));
}

void ControllerProcessAux::createSuicideProcess(){
    cout << "Creating " << fileName  << endl;
    pipe(f);
    while(lives > 0 || INFINITE){
        pid = fork();
        string executable = filePath + "/" + fileName;
        switch(pid){
            case 0:
                execl(executable.c_str(), fileName.c_str(), NULL);
                cout << "Error" << endl;
                break;
            default:
                int childStatus;
                waitpid(pid, &childStatus, 0);
                ofstream outputFile("log.txt", fstream::in | fstream::out | fstream::app);
                if(!INFINITE){
                    lives--;
                    outputFile << "Suicide process " << fileName << " ended because of " << childStatus << 
                    " -- Controller process " << getpid() << ", remaining lives: "<< lives << endl;
                }else{
                    outputFile << "Suicide process " << fileName << " ended because of " << childStatus << 
                    " -- Controller process " << getpid() << ", remaining lives: Infinite " << endl;
                }
                outputFile.close();
                break;
                /*while(lives > 0){
                    int childStatus;
                    waitpid(pid, &childStatus, 0);
                    lives--;
                    cout << "Suicide process " << fileName << " ended because of " << childStatus << 
                    " -- Controller process " << getpid() << ", remaining lives "<< lives << endl;
                }*/
                
        }
    }
}

void ControllerProcessAux::getOperation(string command, string id, string number){
    int code;
    map<string, int> commands;
    commands["listar"] = 0;
    commands["sumar"] = 1;
    commands["restar"] = 2;
    commands["suspender"] = 3;
    commands["restablecer"] = 4;
    commands["indefinir"] = 5;
    commands["definir"] = 6;
    commands["terminar"] = 7;
    code = commands.find(command)->second;
    int num; 
    switch(code){
        case 0:
            list();
            break;
        case 1:
            num = stoi(number);
            sum(num);
            break;
        case 2:
            num = stoi(number);
            sub(num);
            break;
        case 3:
            suspend();
            break;
        case 4:
            restore();
            break;
        case 5:
            undefine();
            break;
        case 6:
            num = stoi(number);
            define(num);
            break;
        case 7:
            end();
            break;
      default:
        break;
   }
}


void ControllerProcessAux::list(){
    cout << "Controller process " << getpid() << "\n"
         << "\tSuicide process " << fileName << "\n"
         << "\tFile path: " << filePath << "\n"
         << "\tId memory: " << idMem << "\n"
         << "\tId semaphore: " << idSem << "\n";
    if(INFINITE){
        cout << "\tLives: Infinite." << endl;
    }else{
        cout << "\tLives: " << lives << endl;
    }
}

void ControllerProcessAux::sum(int num){
    if(INFINITE){
        cout << "Suicide process " << fileName << " couldn't add more lives "
        " -- Controller process " << getpid() << ", remaining lives: Infinite" << endl;
    }else{
        this->lives = this->lives + num;
        cout << "Suicide process " << fileName << " has added " << num << 
        " lives -- Controller process " << getpid() << ", remaining lives: "<< lives << endl;
    }
}

void ControllerProcessAux::sub(int num){
    if(INFINITE){
            cout << "Suicide process " << fileName << " couldn't lose lives "
            " -- Controller process " << getpid() << ", remaining lives: Infinite" << endl;
        }else{
            this->lives = this->lives - num;
            cout << "Suicide process " << fileName << " has lost " << num << 
            " lives -- Controller process " << getpid() << ", remaining lives "<< lives << endl;
        }

}

void ControllerProcessAux::suspend(){
    cout << "Suicide process " << fileName << " has been suspended " <<
    " -- Controller process " << getpid() << ", remaining lives: "<< lives << endl;
}

void ControllerProcessAux::restore(){
    cout << "Suicide process " << fileName << " has been restored " <<
    " -- Controller process " << getpid() << ", remaining lives: "<< lives << endl;
}

void ControllerProcessAux::undefine(){
    if(INFINITE){
        cout << "Suicide process " << fileName << " already had infinite lives " <<
        " -- Controller process " << getpid() << ", remaining lives: Infinite" << endl;
    }else{
        this->lives = 0;
        INFINITE = true;
        cout << "Suicide process " << fileName << " has been undefined " <<
        " -- Controller process " << getpid() << ", remaining lives: Infinite" << endl;
    }
}

void ControllerProcessAux::define(int num){
    if(INFINITE){
        INFINITE = false;
    }
    this->lives = num;
    cout << "Suicide process " << fileName << " has been defined " <<
    " -- Controller process " << getpid() << ", remaining lives "<< lives << endl;
}
void ControllerProcessAux::end(){
    cout << "Suicide process " << fileName << " has been ended " << endl;
}