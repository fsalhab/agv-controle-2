#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include "headers/comms_control.hpp"

#define COMMS_LOG 1   // create logs
#define COMMS_DEBUG 1 // log to stdout
using namespace std;

float Rang()
{
    cout << "entrei no Rang";
    //recebe o angulo da parte de processamento de imagem
    comms::msgStruct msg;
    msg = comms::getData();
    cout << "passei do getdata";
    return msg.ang;
}

int main(int argc, char const *argv[])
{
    float ang;
    comms::initComms("localhost", 5556);
    std::cout << "connected" << '\n';
    while (true)
    {
        cout << "entrei no while";
        ang = Rang();
        cout << "o valor de angulo recebido foi" << ang;
        sleep(1);
    }
}
