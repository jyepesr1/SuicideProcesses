#include "ControllerProcessAux.h"

ControllerProcessAux::ControllerProcessAux(string filePath, string fileName, string lives, int idMem, int idSem){
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

void ControllerProcessAux::setId(string id){
    this->id = id;
}

void ControllerProcessAux::readBuffer(){
    char readbuffer[1024];
    close(fd[0][WRITE_END]);
    read(fd[0][READ_END], readbuffer, sizeof(readbuffer));
}

void ControllerProcessAux::createSuicideProcess(){
    unique_lock<mutex> lock(mutExecution);
    //pipe(fd[0]);
    while(lives > 0 || INFINITE){
        while(!executionStatus){
            executionVar.wait(lock);
        }
        pid = fork();
        string executable = filePath + "/" + fileName;
        switch(pid){
            case 0:
      /*          close(fd[0][READ_END]);
                dup2(fd[0][WRITE_END], WRITE_END);
                close(fd[1][WRITE_END]);
                dup2(fd[1][READ_END], READ_END);*/
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
                    
                    cout << "Suicide process " << fileName << " ended because of " << childStatus << 
                    " -- Controller process " << getpid() << ", remaining lives: "<< lives << endl;
                
                }else{
                    outputFile << "Suicide process " << fileName << " ended because of " << childStatus << 
                    " -- Controller process " << getpid() << ", remaining lives: Infinite " << endl;
                    
                    cout << "Suicide process " << fileName << " ended because of " << childStatus << 
                    " -- Controller process " << getpid() << ", remaining lives: Infinite " << endl;
                    
                }
                sem_lock();
                writeSharedMemory();
                sem_unlock();
                
                outputFile.close();
                if(lives == 0 && !INFINITE){
                    end();
                }
                break;
                
        }
    }
}

void ControllerProcessAux::getOperation(string command, string number){
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
    cout << "Controller process " << getpid() << ": "
         << "Suicide process " << fileName << ", "
         << "File path: " << filePath << ", "
         << "Id memory: " << idMem << ", "
         << "Id semaphore: " << idSem << ", ";
    if(INFINITE){
        cout << "Lives: Infinite" << endl;
    }else{
        cout << "Lives: " << lives << endl;
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
    executionStatus = false;
    cout << "Suicide process " << fileName << " has been suspended " <<
    " -- Controller process " << getpid() << ", remaining lives: "<< lives << endl;
}

void ControllerProcessAux::restore(){
    unique_lock<mutex> lock(mutExecution);
    executionStatus = true;
    executionVar.notify_one();
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
    cout << "Controller process " << getpid() << " ended " << endl;
    try{
        exit(1);
    }catch(int &z){
        cout << "Troubles" << endl;
    }
}

void ControllerProcessAux::initializeSharedMemory(){
   int key = ftok("../examples/shm", idMem);
   if (key == -1) {
      cerr << "Error with key \n" << endl;
      exit(1); 
   }
   
   int memSize = sizeof(long int) + sizeof(InfoMuerte*) + (256 + sizeof(int))*4;
   
   this->id_MemZone = shmget(key, memSize, 0666);
   if (id_MemZone == -1) {
      fprintf (stderr, "Error with id_MemZone Reading\n");
      exit(1); 
   }
   
   /* shared sharedMemory */
   /* we declared to zone to share */
   this->sharedMemory = (MemoriaCompartida *)shmat (id_MemZone, (char *)0, 0);
   if (sharedMemory == NULL) {
      fprintf (stderr, "Error reserve shared memory \n");
      exit(1);
   }
}

void ControllerProcessAux::writeSharedMemory(){
   
    sharedMemory->muertes = (InfoMuerte* )(sharedMemory + 1);
    sharedMemory->valSeq++;
    bool searching = true;
    int i = 0;
    while(searching){
        if(strcmp(sharedMemory->muertes[i].id, this->id.c_str()) == 0){
            sharedMemory->muertes[i].nDecesos++;
            searching = false;
        }
        i++;
    }
}

void ControllerProcessAux::sem_lock(){
    /* structure for semaphore operations.   */
    struct sembuf sem_op;

    /* wait on the semaphore, unless it's value is non-negative. */
    sem_op.sem_num = 0;
    sem_op.sem_op = -1;
    sem_op.sem_flg = 0;
    semop(sem_id, &sem_op, 1);
}

void ControllerProcessAux::sem_unlock(){
    /* structure for semaphore operations.   */
    struct sembuf sem_op;

    /* signal the semaphore - increase its value by one. */
    sem_op.sem_num = 0;
    sem_op.sem_op = 1;
    sem_op.sem_flg = 0;
    semop(sem_id, &sem_op, 1);
}

void ControllerProcessAux::initializeSem(){
    union semun {              /* semaphore value, for semctl().     */
        int val;
        struct semid_ds *buf;
        ushort * array;
    } sem_val;    
    
    /* create a semaphore set, with one semaphore   */
    sem_id = semget(idSem, 1, IPC_CREAT | 0666);
    if(sem_id == -1){
        cerr << "Error creating semaphore" << endl;
        exit(1);
    }
    
    /* intialize the first (and single) semaphore in our set to '1'. */
    sem_val.val = 1;
    int returnValue = semctl(sem_id, 0, SETVAL, sem_val);
    if (returnValue == -1) {
        cerr << "Error initializing semaphore" << endl;
	    exit(1);
    }
}