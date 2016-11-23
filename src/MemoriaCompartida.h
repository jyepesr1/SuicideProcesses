#pragma once

const int BUFFERSIZE = 256;

struct InfoMuerte {
    char id[BUFFERSIZE];
    int nDecesos;
};

struct MemoriaCompartida {
    long int valSeq;
    InfoMuerte* muertes;
};