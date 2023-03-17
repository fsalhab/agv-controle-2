#include <stdio.h>
#include <wiringPi.h>
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

//Configuração pinos
int gpIO[4] = {1,2,3,4};
int dir_gpIO[4] = {5,6,7,8};

pinMode(gpIO[0],PWM_OUTPUT);
pinMode(dir_gpIO[0],OUTPUT);

pinMode(gpIO[1],PWM_OUTPUT);
pinMode(dir_gpIO[1],OUTPUT);

pinMode(gpIO[2],PWM_OUTPUT);
pinMode(dir_gpIO[2],OUTPUT);

pinMode(gpIO[3],PWM_OUTPUT);
pinMode(dir_gpIO[3],OUTPUT);

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
    sleep(50);
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

/*void segueLinha(float dist, float ang){//distancia e angulo da linha
        for(int i = 0; i < 4; i++){ //Loop que passa por cada roda.
        //roda 0 -> superior direita, roda 1 -> superior esquerda, roda 2 -> inferior direita, roda 3 -> inferior esquerda.
            float speed = force; //essa força é a velocidade que as rodas rodam quando angulo e distancia = 0. Ou seja, a força máxima de cada roda.
            float moduloAng = modulo(ang);
            if(ang > offset2 && (i == 0 || i == 2)){//offset2 é o maior angulo que o agv interpreta como desprezivel: Caso o angulo seja menor que offset2, é desnecessário corrigir o angulo
                speed = force * (1 - ang/anguloMaxLeitura * k2); //k2 multiplica o angulo de entrada, fazendo com que um angulo menor resulte em um maior ângulo de saída do agv.
            }
            if(ang < -1*offset2 && (i == 1 || i == 3)){
                speed = force * (1 - ang/-1*anguloMaxLeitura * k2);//anguloMaxLeitura é o angulo maximo em graus que pode ser lido pelo sensor da linha
            }
            if(moduloAng < offset2 && dist > offset && (i == 0 || i == 3)){ //offset é a maior distancia que o agv interpreta como desprezivel.
                speed = force * (1 - dist/maiordistancia * k1); //maiordistancia é a maior distancia que a câmera enxerga para cada lado.
            }
            if(moduloAng < offset2 && dist < -1*offset && (i == 1 || i == 2)){
                speed = force * (1 - dist/-1*maiordistancia * k1); //k1 decide o angulo maximo que o agv anda diagonalmente. 0 -> não anda diagonalmente, 1 -> 45 graus..
            }
            
            bool sentido = true;
            /*if(speed < 0){ //A principio foi decidido que nao utilizaremos o sentido contrario das rodas.
                speed = 0;
            }
            setSpeedMotor(i, speed, sentido);
        }
    }*/

void corrigirDistancia(float dist, float ang){
    float vx = dist;
    float vy = absf(dist * 1/tan(ang));
    norma = sqrtf((powf(vx, 2) + powf(vy, 2)));
    float vxnormal = vx / norma;
    float vynormal = vy / norma;
    float f = vxnormal + vynormal;
    float e = f - (2 * v.x / sqrt(2)); 
    bool sentido = true;
    pwmWrite(1, e/2);
    digitalWrite(5, HIGH);
    pwmWrite(2, f/2);
    digitalWrite(6, HIGH);
    pwmWrite(3, f/2);
    digitalWrite(7, HIGH);
    pwmWrite(4, e/2);
    digitalWrite(8, HIGH);
}

void corrigirAngulo(float dist, float ang){
    float velocidade = 1;
    if(angulo > 0){
        pwmWrite(1, -1*velocidade);
        digitalWrite(5, HIGH);
        pwmWrite(2, velocidade);
        digitalWrite(6, HIGH);
        pwmWrite(3, velocidade);
        digitalWrite(7, HIGH);
        pwmWrite(4, -1*velocidade);
        digitalWrite(8, HIGH);
    }else{
        if(angulo < 0){
            pwmWrite(1, velocidade);
            digitalWrite(5, HIGH);
            pwmWrite(2, -1*velocidade);
            digitalWrite(6, HIGH);
            pwmWrite(3, -1*velocidade);
            digitalWrite(7, HIGH);
            pwmWrite(4, velocidade);
            digitalWrite(8, HIGH);
        }
    }
}

void Desligar()
{
    exit(0);
}

void Remergencia()
{
    char *nome_var = getenv("STATUS_AGV");
    int status = stoi(nome_var);
    if (status == EMERGENCIA)
    {
        Desligar();
    }
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
        Remergencia();
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
        corrigirDistancia(dist,ang);

        corrigirAngulo(dist,ang);
        //segueLinha(dist, ang);
        fim = Rfim();
    }
    Enviastatus(FIM);
    Desligar();
}