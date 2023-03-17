#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include "headers/comms_control.hpp"

#define COMMS_LOG 1   // create logs
#define COMMS_DEBUG 1 // log to stdout
using namespace std;

void Edist(float dist)
{
    string nome_variavel = "DIST_AGV";
    string novo_valor = to_string(dist);
    string variavel = nome_variavel + "=" + novo_valor;

    int resultado = putenv(variavel.c_str());
}

int main(int argc, char const *argv[])
{
    float dist;
    dist = 7.7;
    Edist(dist);
    char *nome_var = getenv("DIST_AGV");
    float valor = stof(nome_var);
    if (valor == 7.7)
    {
        cout << "funcionou";
    }
}
