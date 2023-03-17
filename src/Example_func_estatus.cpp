#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include "headers/comms_control.hpp"

#define COMMS_LOG 1   // create logs
#define COMMS_DEBUG 1 // log to stdout
using namespace std;

void Enviastatus(int estado)
{
    // Definindo a função que envia o status para o supervisório
    if (estado == PARADO)
    {
        cout << "O Avg está parado" << endl;
    }
    else if (estado == EM_MOVIMENTO)
    {
        cout << "O Avg está em movimento" << endl;
    }
    else if (estado == FIM)
    {
        cout << "O Avg finalizou o percurso" << endl;
    }
    string nome_variavel = "STATUS_AGV";
    string novo_valor = to_string(estado);
    string variavel = nome_variavel + "=" + novo_valor;
    int resultado = putenv(variavel.c_str());
}

int main(int argc, char const *argv[])
{
    int estado;
    estado = 1;
    Enviastatus(estado);
    char *nome_var = getenv("STATUS_AGV");
    int valor = stoi(nome_var);
    if (valor == 1)
    {
        cout << "funcionou";
    }
}
