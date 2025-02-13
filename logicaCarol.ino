#include <IRremote.h>
const int PINO_ENA = 3; //pwm A
const int PINO_ENB = 9; //pwm B
// Declaração dos pinos utilizados para controlar o sentido do motor
const int PINO_IN1 = 5; // Primeiro motor
const int PINO_IN2 = 4;
const int PINO_IN3 = 7; // Segundo motor
const int PINO_IN4 = 8;
const int sensorInfraT = A2; //sensores infra-vermelho
const int sensorInfraE = A1;
const int sensorInfraD = A3;
const int sensorDisE = A5; //sensores de distancia
const int sensorDisD = A7;
const int sensorDisF = A6;
int leituraDisF = 0; //valor da leitura dos sensores de distância
int leituraDisE = 0; 
int leituraDisD = 0;
int valorDisF = 0; //valor da leitura dos sensores de distância convertido em cm
int valorDisE = 0;
int valorDisD = 0;
int leituraInfraT = 0; //valor da leitura dos sensores infra-vermelho
int leituraInfraE = 0;
int leituraInfraD = 0;
float somaT, mediaT;
float somaE, mediaE;
float somaD, mediaD;
#define standby 6
int cont = 0;
//const int IRreceiver = 3;
//unsigned long valor_remoto;
int vetorDisF[3], vetorDisE[3], vetorDisD[3];
const int sensorInfra = 11;
unsigned long tempoParada = 0;
const unsigned long intervaloLeitura = 100; // intervalo de 1 segundo


unsigned long tempoAnterior = 0;
int estado = 0;
/*#define power 128
#define numero2 129
#define numero3 130*/

/*void Irremote(){
if(IrReceiver.decode()){
  valor_remoto = IrReceiver.decodedIRData.decodedRawData;
  IrReceiver.resume();
  Serial.println(IrReceiver.decodedIRData.decodedRawData);
}
}*/

void leituraDis(){
leituraDisE = analogRead(sensorDisE);
valorDisE = 13*pow(leituraDisE * 0.0048828125 , -1);

leituraDisD = analogRead(sensorDisD);
valorDisD = 13*pow(leituraDisD * 0.0048828125 , -1);  

leituraDisF = analogRead(sensorDisF);
valorDisF = 13*pow(leituraDisF * 0.0048828125 , -1);
}

float leituraIfE(){
  leituraInfraE = analogRead(sensorInfraE);
  somaE = 0, mediaE = 0;
   for(int c = 0; c<6 ; c++){
    somaE= somaE + leituraInfraE;
   }
  mediaE = somaE/6;
  /*Serial.println("media E: ");
  Serial.println(mediaE);*/
return mediaE;
}
float leituraIfD(){
  leituraInfraD = analogRead(sensorInfraD);
  somaD = 0, mediaD = 0;
   for(int c = 0; c<6 ; c++){
     somaD= somaD + leituraInfraD;
   }
  mediaD = somaD/6;
  /*Serial.println("media D: ");
  Serial.println(mediaD);*/  
  return mediaD;
}
float leituraIfT(){
  //                ||||INICIO SENSORES INFRAVERMELHOS||||
  leituraInfraT = analogRead(sensorInfraT);
  
  somaT = 0, mediaT = 0;
      
    for(int c = 0; c<6 ; c++){
     somaT= somaT + leituraInfraT;
    }
  mediaT = somaT/6;
  /*Serial.println("media T: ");
  Serial.println(mediaT);*/

return mediaT;
//              ||||FIM SENSORES INFRAVERMELHOS||||
           
}

void busca(){
  if (tempoParada == 0) { 
            tempoParada = millis();
        }
        analogWrite(PINO_ENA, 170);
        analogWrite(PINO_ENB, 90);
        digitalWrite(PINO_IN1, HIGH); //MOTOR E
        digitalWrite(PINO_IN2, LOW); //MOTOR E
        digitalWrite(PINO_IN3, LOW); //MOTOR D
        digitalWrite(PINO_IN4, HIGH); //MOTOR D
      if (millis() - tempoAnterior > intervaloLeitura) { // verifica se já passou o intervalo desde a última leitura  
        gira();
    }
}


void gira(){
  analogWrite(PINO_ENA, 150);
  analogWrite(PINO_ENB, 150);
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);
  
}
void re(){
 // Serial.println("re");
  analogWrite(PINO_ENA, 250);
  analogWrite(PINO_ENB, 170);
  digitalWrite(PINO_IN1, HIGH);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);  
}
void frente() {
//  Serial.println("frente");
  analogWrite(PINO_ENA, 250);
  analogWrite(PINO_ENB, 170);
  digitalWrite(PINO_IN1, HIGH); //MOTOR E
  digitalWrite(PINO_IN2, LOW); //MOTOR E
  digitalWrite(PINO_IN3, LOW); //MOTOR D
  digitalWrite(PINO_IN4, HIGH); //MOTOR D
}
void esquerda() {
 // Serial.println("esquerda");
  analogWrite(PINO_ENA, 250);
  analogWrite(PINO_ENB, 170);
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);
}

void direita() {
  //Serial.println("direita");
  analogWrite(PINO_ENA, 250);
  analogWrite(PINO_ENB, 170);
  digitalWrite(PINO_IN1, HIGH);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);
}
void parar() {
 // Serial.println("parar");
  analogWrite(PINO_ENA, 0);
  analogWrite(PINO_ENB, 0);
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, LOW);
}

void frenteR() {
 // Serial.println("frenteR");
  analogWrite(PINO_ENA, 170);
  analogWrite(PINO_ENB, 150);
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);
}
void esquerdaR() {
  //Serial.println("esquerdaR");
  analogWrite(PINO_ENA, 150);
  analogWrite(PINO_ENB, 150);
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);
}

void direitaR() {
 // Serial.println("direitaR");
  analogWrite(PINO_ENA, 150);
  analogWrite(PINO_ENB, 150);
  digitalWrite(PINO_IN1, HIGH);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);
}

void setup () {
  pinMode(sensorInfra, INPUT);
  Serial.begin(9600);

  pinMode(PINO_ENA, OUTPUT);
  pinMode(PINO_ENB, OUTPUT);
  pinMode(PINO_IN1, OUTPUT);
  pinMode(PINO_IN2, OUTPUT);
  pinMode(PINO_IN3, OUTPUT);
  pinMode(PINO_IN4, OUTPUT);
  pinMode(sensorInfraT, INPUT);
  pinMode(sensorInfraD, INPUT);
  pinMode(sensorInfraE, INPUT);
  pinMode(sensorDisE, INPUT);
  pinMode(sensorDisD, INPUT);
  pinMode(sensorDisF, INPUT);
  digitalWrite(standby, HIGH);
  digitalWrite(PINO_IN1, LOW); 
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, LOW);
  digitalWrite(PINO_ENA, LOW);
  digitalWrite(PINO_ENB, LOW);
  Serial.begin(9600);
  
  /*IrReceiver.begin(IRreceiver, ENABLE_LED_FEEDBACK);
      Serial.begin(9600); // Inicializa a comunicação serial a 9600 bps
    pinMode(power, INPUT_PULLUP);   // Configura o pino como entrada com pull-up interno
    pinMode(numero2, INPUT_PULLUP); // Configura o pino como entrada com pull-up interno
    pinMode(numero3, INPUT_PULLUP);*/
}

void loop () {
  int val = digitalRead(sensorInfra);
  Serial.println(val);
  if(val == 1){

    for(int c = 0; c < 3; c++){
      leituraDisE = analogRead(sensorDisE);
      valorDisE = 13*pow(leituraDisE * 0.0048828125 , -1);
      vetorDisE[c] = valorDisE;
      Serial.print("vetor esquerda: ");
      Serial.println(vetorDisE[c]);
    }
    for(int c = 0; c < 3; c++){
      leituraDisD = analogRead(sensorDisD);
      valorDisD = 13*pow(leituraDisD * 0.0048828125 , -1); 
      vetorDisD[c] = valorDisD; 
      Serial.print("vetor direita: ");
      Serial.println(vetorDisD[c]);      
    }
    for(int c = 0; c < 3; c++){
      leituraDisF = analogRead(sensorDisF);
      valorDisF = 13*pow(leituraDisF * 0.0048828125 , -1);
      vetorDisF[c] = valorDisF;
      Serial.print("vetor frente: ");
      Serial.println(vetorDisF[c]);      
    }
  
  /*Serial.println("distancia da frente: ");
  Serial.println(valorDisF);

  Serial.println("distancia da esquerda: ");
  Serial.println(valorDisE);

  Serial.println("distancia da direita: ");
  Serial.println(valorDisD);*/
  
      
      while((vetorDisF[0] > 23 || vetorDisF[1] > 23 || vetorDisF[2] > 23) && (vetorDisE[0] > 23 || vetorDisE[1] > 23 || vetorDisE[2] > 23) && (vetorDisD[0] > 23 || vetorDisD[1] > 23 || vetorDisD[2] > 23)){
        //Serial.println("não achou nada");
        busca();       
        mediaT = leituraIfT();          
        mediaD = leituraIfD();
        mediaE = leituraIfE();

          for(int c = 0; c < 3; c++){
            leituraDisE = analogRead(sensorDisE);
            valorDisE = 13*pow(leituraDisE * 0.0048828125 , -1);
            vetorDisE[c] = valorDisE;
          }
          for(int c = 0; c < 3; c++){
            leituraDisD = analogRead(sensorDisD);                     //PARTE DO SENSOR DE DISTÂNCIA
            valorDisD = 13*pow(leituraDisD * 0.0048828125 , -1); 
            vetorDisD[c] = valorDisD; 
          }
          for(int c = 0; c < 3; c++){
            leituraDisF = analogRead(sensorDisF);
            valorDisF = 13*pow(leituraDisF * 0.0048828125 , -1);
            vetorDisF[c] = valorDisF;
          }

         /* if(mediaT < 680 || mediaE < 650 || mediaD < 650){
            Serial.println("entrou no goto de procura");
            goto infraNoBranco;  
          }*/      
      }   

      if((vetorDisF[0] <= 23 && vetorDisF[0] >3) && (vetorDisF[1] <= 23 && vetorDisF[1] >3) && (vetorDisF[2] <= 23 && vetorDisF[2]>3)){
        frente();        
        //Serial.println("achou longe da frente");
      }

      else if((vetorDisD[0] <= 23 && vetorDisD[0] > 3) && (vetorDisD[1] <= 23 && vetorDisD[1] >3) && (vetorDisD[2] <= 23 && vetorDisD[2] >3)){
        //Serial.println("achou longe na direita");
        
        while((vetorDisF[0] > 23 && vetorDisF[0] >3) || (vetorDisF[1] > 23 && vetorDisF[1] >3) || (vetorDisF[2] > 23 && vetorDisF[2] >3)){
         // Serial.println("esta virando da direita pra frente");
          direita();
          mediaT = leituraIfT();          
          mediaD = leituraIfD();
          mediaE = leituraIfE();

          for(int c = 0; c < 3; c++){
            leituraDisE = analogRead(sensorDisE);
            valorDisE = 13*pow(leituraDisE * 0.0048828125 , -1);
            vetorDisE[c] = valorDisE;
          }
          for(int c = 0; c < 3; c++){
            leituraDisD = analogRead(sensorDisD);                     //PARTE DO SENSOR DE DISTÂNCIA
            valorDisD = 13*pow(leituraDisD * 0.0048828125 , -1); 
            vetorDisD[c] = valorDisD; 
          }
          for(int c = 0; c < 3; c++){
            leituraDisF = analogRead(sensorDisF);
            valorDisF = 13*pow(leituraDisF * 0.0048828125 , -1);
            vetorDisF[c] = valorDisF;
            //Serial.println(vetorDisF[c]);
          }

          /*if(mediaT < 680 || mediaE < 630  || mediaD < 650){
            Serial.println("entrou no goto de virar da direita pra frente");
            goto infraNoBranco;
              
          } */   
        }              
        frente();        
       //Serial.println("trocou da direita pra frente");        
      }

      else if((vetorDisE[0] <= 23 && vetorDisE[0] >3) && (vetorDisE[1] <= 25 && vetorDisE[1] >3) && (vetorDisE[2] <= 25 && vetorDisE[2] >3)){
        //Serial.println("achou longe na esquerda");
        while((vetorDisF[0] > 25 && vetorDisF[0] >3) || (vetorDisF[1] > 25 && vetorDisF[1] >3) || (vetorDisF[2] > 25 && vetorDisF[2] >3)){
         // Serial.println("trocando da esquerda pra frente");
          esquerda();
          mediaT = leituraIfT();
          mediaD = leituraIfD();
          mediaE = leituraIfE();

          for(int c = 0; c < 3; c++){
            leituraDisE = analogRead(sensorDisE);
            valorDisE = 13*pow(leituraDisE * 0.0048828125 , -1);
            vetorDisE[c] = valorDisE;
          }
          for(int c = 0; c < 3; c++){
            leituraDisD = analogRead(sensorDisD);                     //PARTE DO SENSOR DE DISTÂNCIA
            valorDisD = 13*pow(leituraDisD * 0.0048828125 , -1); 
            vetorDisD[c] = valorDisD; 
          }
          for(int c = 0; c < 3; c++){
            leituraDisF = analogRead(sensorDisF);
            valorDisF = 13*pow(leituraDisF * 0.0048828125 , -1);
            vetorDisF[c] = valorDisF;
          }

          /*if(mediaT < 680 || mediaE < 630 || mediaD < 650){
            Serial.println("entrou no goto de virar da esquerda pra frente");
            goto infraNoBranco;  
          }*/    
        }
        frente();         
        //Serial.println("trocou da esquerda pra frente");
      }

      else if((vetorDisE[0] <= 25 && vetorDisE[0] >3) && (vetorDisE[1] <= 25 && vetorDisE[1] >3) && (vetorDisE[2] <= 25 && vetorDisE[2] >3) && (vetorDisD[0] <= 25 && vetorDisD[0] >3) && (vetorDisD[1] <= 25 && vetorDisD[1] >3) && (vetorDisD[2] <= 25 && vetorDisD[2] >3)){
        
        while((vetorDisF[0] > 25 && vetorDisF[0] >3) || (vetorDisF[1] > 25 && vetorDisF[1] >3) || (vetorDisF[2] > 25 && vetorDisF[2] >3)){
         // Serial.println("esa virando da direita pra frente os 2 tao vendo");
          direita();
          mediaT = leituraIfT();          
          mediaD = leituraIfD();
          mediaE = leituraIfE();

          for(int c = 0; c < 3; c++){
            leituraDisE = analogRead(sensorDisE);
            valorDisE = 13*pow(leituraDisE * 0.0048828125 , -1);
            vetorDisE[c] = valorDisE;
          }
          for(int c = 0; c < 3; c++){
            leituraDisD = analogRead(sensorDisD);                     //PARTE DO SENSOR DE DISTÂNCIA
            valorDisD = 13*pow(leituraDisD * 0.0048828125 , -1); 
            vetorDisD[c] = valorDisD; 
          }
          for(int c = 0; c < 3; c++){
            leituraDisF = analogRead(sensorDisF);
            valorDisF = 13*pow(leituraDisF * 0.0048828125 , -1);
            vetorDisF[c] = valorDisF;
            //Serial.println(vetorDisF[c]);
          }

          /*if(mediaT < 680 || mediaE < 630  || mediaD < 650){
            Serial.println("entrou no goto de virar da direita pra frente");
            goto infraNoBranco;
              
          }*/    
        }
      }
     
      /*else if(valorDisF <= 10){
        frenteR(); 
        //Serial.println("achou perto na frente");
      }

      else if(valorDisD <= 10){
       // Serial.println("achou perto na direita");
        while(valorDisF>25){
          //Serial.println("girando perto da direita pra frente");
          direitaR();
          mediaT = leituraIfT();
          mediaD = leituraIfD();
          mediaE = leituraIfE();

            leituraDisE = analogRead(sensorDisE);
  valorDisE = 13*pow(leituraDisE * 0.0048828125 , -1);

  leituraDisD = analogRead(sensorDisD);
  valorDisD = 13*pow(leituraDisD * 0.0048828125 , -1);  

  leituraDisF = analogRead(sensorDisF);
  valorDisF = 13*pow(leituraDisF * 0.0048828125 , -1);

            if(mediaT < 680 && mediaE < 685  && mediaD < 650){
             // Serial.println("entrou no goto de virar rapido da direita pra frente");
              goto infraNoBranco;  
            }                                 
        }
        frenteR(); 
        //Serial.println("trocou na direita perto pra frente");
      }

      else if(valorDisE <= 10){
       // Serial.println("achou perto na esquerda");
        while(valorDisF>25){
          //Serial.println("girando perto da esquerda pra frente");
          esquerdaR();
          mediaT = leituraIfT();
          mediaD = leituraIfD();
          mediaE = leituraIfE();

            leituraDisE = analogRead(sensorDisE);
  valorDisE = 13*pow(leituraDisE * 0.0048828125 , -1);

  leituraDisD = analogRead(sensorDisD);
  valorDisD = 13*pow(leituraDisD * 0.0048828125 , -1);  

  leituraDisF = analogRead(sensorDisF);
  valorDisF = 13*pow(leituraDisF * 0.0048828125 , -1);

            if(mediaT < 680 && mediaE < 685  && mediaD < 650){
              //Serial.println("entrou no goto de virar rapido da esquerda pra frente");
              goto infraNoBranco;  
            }
        }
        frenteR();  
        //Serial.println("trocou na esquerda perto pra frente");    
      }*/
    
    // Criar funções de reação: Um robô esta se aproximando, devemos aumentar a velocidade pra criar resistencia ao ser empurrado?
    //}
    }
    
    
  /*else if (val == 0){
    parar();
    val = digitalRead(sensorInfra)+ 
    1;
    
  }*/
  
  else if(val == 0){
    parar();
  }
}
