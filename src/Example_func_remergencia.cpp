#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include "headers/comms_control.hpp"

#define COMMS_LOG 1   // create logs
#define COMMS_DEBUG 1 // log to stdout
using namespace std;

void Remergencia()
{
    char *nome_var = getenv("STATUS_AGV");
    int status = stoi(nome_var);
    if (status == EMERGENCIA)
    {
        cout << "emergencia, funcionou";
    }
}

int main(int argc, char const *argv[])
{
    int estado;
    estado = 2;
    string nome_variavel = "STATUS_AGV";
    string novo_valor = to_string(estado);
    string variavel = nome_variavel + "=" + novo_valor;
    int resultado = putenv(variavel.c_str());
    Remergencia();
}
