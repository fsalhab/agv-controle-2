#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include "headers/comms_control.hpp"

#define COMMS_LOG 1   // create logs
#define COMMS_DEBUG 1 // log to stdout
using namespace std;

float Rdist()
{
    //recebe o angulo da parte de processamento de imagem
    comms::msgStruct msg;
    msg = comms::getData();
    return msg.dist;
}

int main(int argc, char const *argv[])
{
    float dist;
    comms::initComms("localhost", 5556);
    std::cout << "connected" << '\n';
    while (true)
    {
        dist = Rdist();
        cout << "o valor de angulo recebido foi" << dist << "\n";
        sleep(1);
    }
}
