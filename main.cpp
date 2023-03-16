#include <stdio.h>
#include <wired.h>
#include <cstdlib>
#include <stdio.h>

using namespace std;

#define K1 100
#define K2 100

#define MDISTANCIA 100

// Definindo as constantes que representam os estados do AGV
const int PARADO = 0;
const int EM_MOVIMENTO = 1;
const int EMERGENCIA = 2;

float Rang(){
    //recebe e retorna o angulo
}

float Rdist(){
    //recebe e retorna a distancia
}

float Rparada(){
    //recebe se parou
}

int Remergencia(int emergencia){
  //recebe se está ou nao em emergencia, se o botao foi pressionado
  if (emergencia) return EMERGENCIA;
  else return EM_MOVIMENTO;
}

void aguardar(){
    //função para deixar a maquina
}

int Rfim(){
    // recebe se a agv chegou ao fim
}

int parar(){
    // finalizar o percurso
}

void Eang(float ang){
    //envia ang
}

void Edist(float dist){
    //envia dist
}

// Definindo a função que recebe e retorna o estado do AGV
void Enviastatus(int estado) {
  if (estado == PARADO) {
    cout << "O Avg está parado" << endl;
    //aqui ele deve enviar o estado para o supervisório
  } else if (estado == EM_MOVIMENTO) {
    cout << "O Avg está em movimento" << endl;
    //aqui ele deve enviar o estado para o supervisório
  } else {
    cout << "O Avg está em emergência" << endl;
    //aqui ele deve enviar o estado para o supervisório
  }
}


void segueLinha(float dist, float ang){

        for(int i = 0; i < 4; i++){ //Loop que passa por cada roda.
            //roda 0 -> superior direita, roda 1 -> superior esquerda, roda 2 -> inferior direita, roda 3 -> inferior esquerda.
            float speed = force; //essa força é a velocidade que as rodas rodam quando angulo e distancia = 0
            if(ang > offset2 && (i == 0 || i == 2)){

                speed = force * (1 - ang/180 * K2); //k2 decide a velocidade em que o agv roda.

            }
            if(ang < -1*offset2 && (i == 1 || i == 3)){

                speed = force * (1 - ang/-180 * K2);
            }
            if(abs(ang) < offset2 && dist > offset && (i == 0 || i == 3)){ //Mathf.Abs() retorna o módulo do valor passado.

                speed = force * (1 - dist/MDISTANCIA * K1); //maiordistancia é a maior distancia que a câmera pega para cada lado.

            }
            if(abs(ang) < offset2 && dist < -1*offset && (i == 1 || i == 2)){

                speed = force * (1 - dist/-1*MDISTANCIA * K1); //k1 decide o angulo maximo que o agv anda diagonalmente.
            }
            bool sentido = true;

            if(speed < 0){

                sentido = false;
                speed = -1*speed;

            }
            setSpeedMotor(i, speed, sentido);
        }
    }
    Void Desligar(){
        //de
    }


int main(){
    //execucao do script de inicializacao

    int fim; 
    int parada = 0;
    float dist,ang;
    
    fim = Rfim();
    while(fim == 0){
        
        dist = Rdist();
        ang = Rang();
        parada = Rparada();
        emergencia = Remergencia();
        Edist(float dist);
        Eang(float ang);

        if (parada == 1){
            Enviastatus(PARADO);
            aguardar();
        }

        if (emergencia == 2){
            Enviastatus(EMERGENCIA);
            break; //escolhemos o break pois o agv para completamente ao apertar o botao de emergencia
        }

        Enviastatus(EM_MOVIMENTO);
        segueLinha(dist,ang);
        fim = Rfim();
    }
    
    Desligar();
}