#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include "headers/comms_control.hpp"

#define COMMS_LOG 1   // create logs
#define COMMS_DEBUG 1 // log to stdout
using namespace std;

void Eang(float ang)
{
    string nome_variavel = "ANG_AGV";
    string novo_valor = to_string(ang);
    string variavel = nome_variavel + "=" + novo_valor;

    int resultado = putenv(variavel.c_str());
}

int main(int argc, char const *argv[])
{
    float ang;
    ang = 7.7;
    Eang(ang);
    char *nome_var = getenv("ANG_AGV");
    float valor = stof(nome_var);
    if (valor == 7.7)
    {
        cout << "funcionou";
    }
}
