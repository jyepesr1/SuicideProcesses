#include <getopt.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
   int arg;
   struct option long_options[] = {
      {"filepath", required_argument, NULL, 'p'},
      {"filename", required_argument, NULL, 'n'},
      {"reencarnacion", required_argument, NULL, 'r'},
      {"memoriacompartida", no_argument, NULL, 'm'},
      {"semaforo", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}
   };
   string filepath, filename, lives;
   while((arg = getopt_long(argc, argv, "pnr:ms::", long_options, NULL)) != -1){
      switch(arg){
         case 'p':
            filepath = optarg;
            break;
         case 'n':
            filename = optarg;
            break;
         case 'r':
            lives = optarg;
            break;
         default:
            cout << "sorry " << endl;
            break;
      }
   }
   
   string line;
   while(cin){
      getline(cin, line);
      getParams(line);
   }
}

void getParams(string inputString){
   
}
