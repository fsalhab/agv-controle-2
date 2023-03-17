using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class andar_agv : MonoBehaviour
{
    Rigidbody rb;
    public float force = 10f, maiordistancia = 0.4f, offset = 0.05f, offset2 = 10, k1 = 2f, k2 = 10f, anguloMaxLeitura = 90;
    //float angulo = Mathf.PI/4;
    public GameObject [] rodas;
    public LayerMask mask;
    float angulo = 0, distancia = 0;
    //public Vector3 closestpointlocal, closestpoint;
    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody>();
        rodas = new GameObject[4];
        for (int i = 0; i < 4; i++){
            rodas[i] = transform.GetChild(i).gameObject;
        }
    }
    void log(string msg){
        Debug.Log(msg);
    }
    // Update is called once per frame
    void FixedUpdate()
    {
        //ler distancia
        RaycastHit esq, dir, def;
        bool hesq = Physics.Raycast(new Vector3(transform.position.x, 0, transform.position.z), -1*transform.right, out esq, maiordistancia, mask);
        bool hdir = Physics.Raycast(new Vector3(transform.position.x, 0, transform.position.z), transform.right, out dir, maiordistancia, mask);
        distancia = 0;
        if(hdir){
            distancia = dir.distance;
            def = dir;
            if(hesq){
                if(esq.distance < dir.distance){
                    distancia = -1*esq.distance;
                    def = esq;
                }
            }
        }else{
            distancia = -1*esq.distance;
            def = esq;
        }
        //ler angulo
        angulo = 0;
        if(hesq || hdir){
            angulo = Vector3.SignedAngle(transform.forward, def.transform.forward, Vector3.up);
        }
        log(""+distancia + " " + angulo);

        //simular resistencia do motor
        rb.velocity = Vector3.zero;
        rb.angularVelocity = Vector3.zero;

        //Chama função de corrigir linha / angulo
        //segueLinha(distancia, angulo)
        segueLinha2(distancia, angulo);
    }

    float modulo(float n){
        if(n < 0){
            return -1*n;
        }
        return n;
    }

    float pow(float a, float b){
        for(int i = 0; i < b; i++){
            a = a * a;
        }
        return a;
    }

    void segueLinha(float dist, float ang){//distancia e angulo da linha
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
            }*/
            setSpeedMotor(i, speed, sentido);
        }
    }

    void segueLinha2(float dist, float ang){
        float f = 1;
        float e = 1;
        float vx = dist;
        float vy = Mathf.Abs(dist * 1/Mathf.Tan(ang));
        Vector2 v = new Vector2(vx, vy);
        v = v.normalized;
        //float norma = Mathf.Sqrt((pow(v.x, 2) + pow(v.y, 2)));
        //v = v / norma;
        //Debug.Log(vx + " " + vy + " " + v);
        if(ang != 0){
            f = (v.x + v.y) / Mathf.Sqrt(2);
            e = f - (2 * v.x / Mathf.Sqrt(2)); 
        } 
        bool sentido = true;
        setSpeedMotor(0, e/2, sentido);
        setSpeedMotor(1, f/2, sentido);
        setSpeedMotor(2, f/2, sentido);
        setSpeedMotor(3, e/2, sentido);
    }

    void setSpeedMotor(int nMotor, float velocidade, bool sentido){
        if(!sentido){
            velocidade = -1*velocidade;
        }
        rb.AddForceAtPosition(rodas[nMotor].transform.forward * velocidade, new Vector3(rodas[nMotor].transform.position.x, 0, rodas[nMotor].transform.position.z), ForceMode.Impulse); 
    }

}
