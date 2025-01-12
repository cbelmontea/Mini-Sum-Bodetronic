#define DECODE_SONY
#include <IRremote.hpp>
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
#define standby2 6
int cont = 0;
const int IRreceiver = 11;
//unsigned long valor_remoto;
int vetorDisF[3], vetorDisE[3], vetorDisD[3];

enum RobotStates {
  standby,
  ready,
  fight,
};

RobotStates robotState = standby;

unsigned long blinkTimer;
bool ledOn;
decode_results results;

void setup() {
  //pinMode(16, OUTPUT);
  Serial.begin(9600);
  IrReceiver.begin(11);
  delay(1000);
  //pinMode(sensorInfra, INPUT);
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
  digitalWrite(standby2, HIGH);
  digitalWrite(PINO_IN1, LOW); 
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, LOW);
  digitalWrite(PINO_ENA, LOW);
  digitalWrite(PINO_ENB, LOW);
}

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

void Blink() {
  if (robotState == ready) {
    Serial.println("Entrou blink");
    if (millis() / 100 > blinkTimer) {
      ledOn = !ledOn;
      digitalWrite(4, ledOn ? HIGH : LOW);
      blinkTimer = (millis() / 100) + 1;
    }
  }
}



void IrReceive() {
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.command, HEX);

    switch (IrReceiver.decodedIRData.command) {
      case 0x0:
        if (robotState == standby) {
          Serial.println("PRONTO!");
          robotState = ready;
        }
        break;
      case 0x1:
        if (robotState == ready) {
          Serial.println("COMBATE!!");
          robotState = fight;
        }
        break;
      case 0x2:
        Serial.println("Standby!!");
        robotState = standby;
        break;
      default:
        Serial.println("Comando desconhecido");
        break;
    }
    IrReceiver.resume();
  }
}

void loop() {
  IrReceive();
  if(robotState == ready){
    Blink();
    Serial.println("ENTROU NO PRIMEIRO IF");
  }
  if(robotState == fight){
    mediaT = leituraIfT();
    mediaD = leituraIfD();
    mediaE = leituraIfE();

    for(int c = 0; c < 3; c++){
      leituraDisE = analogRead(sensorDisE);
      valorDisE = 13*pow(leituraDisE * 0.0048828125 , -1);
      vetorDisE[c] = valorDisE;
     /* Serial.print("vetor esquerda: ");
      Serial.println(vetorDisE[c]);*/
    }
    for(int c = 0; c < 3; c++){
      leituraDisD = analogRead(sensorDisD);
      valorDisD = 13*pow(leituraDisD * 0.0048828125 , -1); 
      vetorDisD[c] = valorDisD; 
    /*  Serial.print("vetor direita: ");
      Serial.println(vetorDisD[c]);     */ 
    }
    for(int c = 0; c < 3; c++){
      leituraDisF = analogRead(sensorDisF);
      valorDisF = 13*pow(leituraDisF * 0.0048828125 , -1);
      vetorDisF[c] = valorDisF;
    /*  Serial.print("vetor frente: ");
      Serial.println(vetorDisF[c]);   */   
    }

  
    if(mediaT >= 680 && mediaE >= 630  && mediaD >= 650){ // Avalia se ta no preto
      
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

    }}
    else{
      infraNoBranco:
     /*  Serial.println("nao reconheceu o preto");
      // Criar funções pra, caso ele identifique o branco consiga voltar pro preto OK
      if(mediaT < 680 && mediaE >= 630 && mediaD >= 650){ // Se ler o branco pelo sensor infra de tras
        frente();    
     *//*   Serial.println("fugiu da borda pelo de tras"); */
        delay(900);
       
      }}
      else if(mediaE < 630 && mediaD >= 650 && mediaT >= 680){ // Se ler o branco pelo sensor infra da esquerda
        parar();
        direita();
     //   Serial.println("fugiu da borda pelo da esquerda");
        delay(900);       
        frente(); // Tira o frente????
          
      }
      else if(mediaD < 650 && mediaE >= 630 && mediaT >= 680){ // Se ler o branco pelo sensor infra da direita
        parar();
        esquerda();
       // Serial.println("fugiu da borda pelo da direita");
        delay(900);
        frente(); // Tira o frente????
       
      }
      else if(mediaD < 650 && mediaE < 630 && mediaT >= 680){
        parar();
        re();
      //  Serial.println("fugiu da borda pelos das laterais"); 
        delay(900); 
      }
      else if(mediaE < 630 && mediaT < 680 && mediaD >= 650){
        direita();
        delay(900);
        frente();
      }
      else if(mediaD < 650 && mediaT < 680 && mediaE >= 630){
        esquerda();
        delay(900);
        frente();
      }
      else if(mediaE < 630 && mediaD < 650 && mediaT < 680){
        re();
        delay(900);
      }      
  }
  if(robotState == standby){
    parar();
    Serial.println("ENTROU NA ULTIMA FUNCAO");
  }
  

  /*IrReceive();
  Blink();
  Fight();
  Stop();*/
}




