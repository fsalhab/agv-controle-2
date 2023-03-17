#include <stdio.h>
#include <wired.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include "headers/comms_control.hpp"

#define COMMS_LOG 1   // create logs
#define COMMS_DEBUG 1 // log to stdout
using namespace std;

#define K1 100
#define K2 100

#define MDISTANCIA 100

// Definindo as constantes que representam os estados do AGV
const int PARADO = 0;
const int EM_MOVIMENTO = 1;
const int EMERGENCIA = 2;
const int FIM = -1;

float Rang()
{
    //recebe o angulo da parte de processamento de imagem
    comms::msgStruct msg;
    msg = comms::getData();
    return msg.ang
}

float Rdist()
{
    //recebe o angulo da parte de processamento de imagem
    comms::msgStruct msg;
    msg = comms::getData();
    return msg.dist
}

int Rparada()
{
    //recebe se a AGV esta em um ponto de parada do processamento de imagem
    comms::msgStruct msg;
    msg = comms::getData();
    return msg.parada
}

void aguardar()
{
    // se estiver num ponto de parada, aguarda 50 segundos
    wait(50);
}

int Rfim()
{
    // recebe se a AGV finalizou o percurso do processamento de imagem
    comms::msgStruct msg;
    msg = comms::getData();
    return msg.fim
}

void Eang(float ang)
{
    string nome_variavel = "ANG_AGV";
    string novo_valor = to_string(ang);
    string variavel = nome_variavel + "=" + novo_valor;

    int resultado = putenv(variavel.c_str());
}

void Edist(float dist)
{
    string nome_variavel = "DIST_AGV";
    string novo_valor = to_string(dist);
    string variavel = nome_variavel + "=" + novo_valor;

    int resultado = putenv(variavel.c_str());
}

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

void segueLinha(float dist, float ang)
{

    for (int i = 0; i < 4; i++)
    { //Loop que passa por cada roda.
        //roda 0 -> superior direita, roda 1 -> superior esquerda, roda 2 -> inferior direita, roda 3 -> inferior esquerda.
        float speed = force; //essa força é a velocidade que as rodas rodam quando angulo e distancia = 0
        if (ang > offset2 && (i == 0 || i == 2))
        {

            speed = force * (1 - ang / 180 * K2); //k2 decide a velocidade em que o agv roda.
        }
        if (ang < -1 * offset2 && (i == 1 || i == 3))
        {

            speed = force * (1 - ang / -180 * K2);
        }
        if (abs(ang) < offset2 && dist > offset && (i == 0 || i == 3))
        { //Mathf.Abs() retorna o módulo do valor passado.

            speed = force * (1 - dist / MDISTANCIA * K1); //maiordistancia é a maior distancia que a câmera pega para cada lado.
        }
        if (abs(ang) < offset2 && dist < -1 * offset && (i == 1 || i == 2))
        {

            speed = force * (1 - dist / -1 * MDISTANCIA * K1); //k1 decide o angulo maximo que o agv anda diagonalmente.
        }
        bool sentido = true;

        if (speed < 0)
        {

            sentido = false;
            speed = -1 * speed;
        }
        setSpeedMotor(i, speed, sentido);
    }
}

Void Desligar()
{
    //de
}

int main(int argc, char const *argv[])
{
    //execucao do script de inicializacao

    int fim;
    int parada = 0;
    float dist, ang;
    comms::initComms("localhost", 5556);
    std::cout << "connected" << '\n';
    fim = Rfim();
    while (fim == 0)
    {

        dist = Rdist();
        ang = Rang();
        parada = Rparada();
        Edist(float dist);
        Eang(float ang);

        if (parada == 1)
        {
            Enviastatus(PARADO);
            aguardar();
        }

        Enviastatus(EM_MOVIMENTO);
        segueLinha(dist, ang);
        fim = Rfim();
    }
    Enviastatus(FIM);
    Desligar();
}