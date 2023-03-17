#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "headers/comms_img.hpp"

#define COMMS_DEBUG            1
#define COMMS_LOG              1

int main (int argc, char const *argv[])
{

    //inicializa a conexao
    comms::initService(5556);
    comms::msgStruct *msg = new comms::msgStruct;

    //leitura variaveis de ambiente
    char* EnvVariables[3];

     EnvVariables[0] = getenv("ANG_AGV");
     EnvVariables[1] = getenv("DIST_AGV");
     EnvVariables[2] = getenv("STATUS_AGV");
     
    msg = comms::getMsgStructRef();
    while (true) { 
        //convertendo char* para int/float com stoi() ou stof();
        msg->ang = stof(EnvVariables[0]);
        msg->dist = stof(EnvVariables[1]);
        msg->parada = stoi(EnvVariables[2]);   
    }

    return 0;
}
