#define DECODE_SONY
#include <IRremote.hpp>

const int PINO_ENA = 33; //pwm A
const int PINO_ENB = 13; //pwm B
// Declaração dos pinos utilizados para controlar o sentido do motor
const int PINO_IN1 = 26; // Primeiro motor
const int PINO_IN2 = 25;
const int PINO_IN3 = 14; // Segundo motor
const int PINO_IN4 = 12;
const int sensorDisE = 35; //sensores de distancia
const int sensorDisD = 34;
const int sensorDisF = 39;
float leituraDisF = 0; //valor da leitura dos sensores de distância
float leituraDisE = 0; 
float leituraDisD = 0;
float valorDisF = 0; //valor da leitura dos sensores de distância convertido em cm
float valorDisE = 0;
float valorDisD = 0;
int vetorDisF[3], vetorDisE[3], vetorDisD[3];
const int Standby = 27;
int cont = 0;
int val;
float limiteMai = 15, limiteMen = 3;
unsigned long tempoParada = 0;
const unsigned long intervaloLeitura = 100; // intervalo de 1 segundo
unsigned long tempoAnterior = 0;
int estado = 0;
const int sensorInfra = 16;
const int configLed = 4;

#define POSITION_NUM 3  // Alterado para 3 chaves
#define ON HIGH   // Agora o estado ativo é HIGH
#define OFF LOW   // Estado inativo é LOW
int valor_switch;
// Define os pinos conectados ao DIP switch
const int SWITCH_PINOS[] = {21, 19, 18};

unsigned long blinkTimer;
bool ledOn;
decode_results results;

enum RobotStates {
  standby,
  ready,
  fight,
};

RobotStates robotState = standby;


void configPisca(){
  switch(valor_switch){
    case 0: //desligado - desligado - desligado
      digitalWrite(configLed, HIGH);
      delay(1000);
      digitalWrite(configLed, LOW);
      delay(1000);
    break;

    case 1: //desligado - desligado - ligado
      digitalWrite(configLed, HIGH);
      delay(1000);
      digitalWrite(configLed, LOW);
      delay(1000);
      digitalWrite(configLed, HIGH);
      delay(1000);
      digitalWrite(configLed, LOW);
      delay(1000);
    break;  

    case 2: //desligado - ligado - desligado
      digitalWrite(configLed, HIGH);
      delay(1000);
      digitalWrite(configLed, LOW);
      delay(1000);
      digitalWrite(configLed, HIGH);
      delay(1000);
      digitalWrite(configLed, LOW);
      delay(1000);
      digitalWrite(configLed, HIGH);
      delay(1000);
      digitalWrite(configLed, LOW);
      delay(1000);
    break;

    default:
      digitalWrite(configLed, LOW);
      delay(1000);
    break;
  }

}

void leituraSwitch(){
  valor_switch = 0;
  for (int i = 0; i < POSITION_NUM; i++) {
    int state = digitalRead(SWITCH_PINOS[i]);

    if (state == ON)  // Agora ON é HIGH
      valor_switch |= 1 << (POSITION_NUM - i - 1);
  }
  Serial.print("encoded state: ");
  Serial.println(valor_switch);
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

void leituraDis(){
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
}

void buscaGeral(){
  analogWrite(PINO_ENA, 150);
  analogWrite(PINO_ENB, 150);
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);
}
void buscaCarol(){
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
        buscaGeral();
    }
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

void logicaPrincipal(){
    leituraDis();  
          
      while((vetorDisF[0] > limiteMai || vetorDisF[1] > limiteMai || vetorDisF[2] > limiteMai) && (vetorDisE[0] > limiteMai || vetorDisE[1] > limiteMai || vetorDisE[2] > limiteMai) && (vetorDisD[0] > limiteMai || vetorDisD[1] > limiteMai || vetorDisD[2] > limiteMai)){
        Serial.println("não achou nada");
        buscaGeral();       

        leituraDis();  
        IrReceive();
          if(robotState == standby){
            loop();
          }  
      }   

      if((vetorDisF[0] <= limiteMai && vetorDisF[0] >limiteMen) && (vetorDisF[1] <= limiteMai && vetorDisF[1] >limiteMen) && (vetorDisF[2] <= limiteMai && vetorDisF[2]>limiteMen)){
        frente();        
        Serial.println("achou longe da frente");
      }

      else if((vetorDisD[0] <= limiteMai && vetorDisD[0] > limiteMen) && (vetorDisD[1] <= limiteMai && vetorDisD[1] >limiteMen) && (vetorDisD[2] <= limiteMai && vetorDisD[2] >limiteMen)){
        Serial.println("achou longe na direita");
        
        while((vetorDisF[0] > limiteMai && vetorDisF[0] >limiteMen) || (vetorDisF[1] > limiteMai && vetorDisF[1] >limiteMen) || (vetorDisF[2] > limiteMai && vetorDisF[2] >limiteMen)){
          Serial.println("esta virando da direita pra frente");
          direita();

          leituraDis();   
          IrReceive();
          if(robotState == standby){
            loop();  
          }
        }              
        frente();        
       Serial.println("trocou da direita pra frente");        
      }

      else if((vetorDisE[0] <= limiteMai && vetorDisE[0] >limiteMen) && (vetorDisE[1] <= limiteMai && vetorDisE[1] >limiteMen) && (vetorDisE[2] <= limiteMai && vetorDisE[2] >limiteMen)){
        Serial.println("achou longe na esquerda");
        while((vetorDisF[0] > limiteMai && vetorDisF[0] >limiteMen) || (vetorDisF[1] > limiteMai && vetorDisF[1] >limiteMen) || (vetorDisF[2] > limiteMai && vetorDisF[2] >limiteMen)){
          Serial.println("trocando da esquerda pra frente");
          esquerda();

          leituraDis(); 
          IrReceive();
          if(robotState == standby){
            loop();
          }
        }
        frente();         
        Serial.println("trocou da esquerda pra frente");
      }

      else if((vetorDisE[0] <= limiteMai && vetorDisE[0] >limiteMen) && (vetorDisE[1] <= limiteMai && vetorDisE[1] >limiteMen) && (vetorDisE[2] <= limiteMai && vetorDisE[2] >limiteMen) && (vetorDisD[0] <= limiteMai && vetorDisD[0] >limiteMen) && (vetorDisD[1] <= limiteMai && vetorDisD[1] >limiteMen) && (vetorDisD[2] <= limiteMai && vetorDisD[2] >limiteMen)){
        
        while((vetorDisF[0] >limiteMai && vetorDisF[0] >limiteMen) || (vetorDisF[1] > limiteMai && vetorDisF[1] >limiteMen) || (vetorDisF[2] > limiteMai && vetorDisF[2] >limiteMen)){
          Serial.println("esa virando da direita pra frente os 2 tao vendo");
          direita();

          leituraDis();
          IrReceive();
          if(robotState == standby){
            loop();   
          }
        }
        frente();
      }
  }

  void logicaPassiva(){

  }

  void logicaAtaqueLento(){

    leituraDis();
     
      while((vetorDisF[0] > 23 || vetorDisF[1] > 23 || vetorDisF[2] > 23) && (vetorDisE[0] > 23 || vetorDisE[1] > 23 || vetorDisE[2] > 23) && (vetorDisD[0] > 23 || vetorDisD[1] > 23 || vetorDisD[2] > 23)){
        //Serial.println("não achou nada");
        buscaGeral();       

          leituraDis();
          IrReceive();
          if(robotState == standby){
            loop();   
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
          leituraDis();
          IrReceive();
          if(robotState == standby){
            loop();   
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
          leituraDis();
          IrReceive();
          if(robotState == standby){
            loop();   
          }    
        }
        frente();         
        //Serial.println("trocou da esquerda pra frente");
      }

      else if((vetorDisE[0] <= 25 && vetorDisE[0] >3) && (vetorDisE[1] <= 25 && vetorDisE[1] >3) && (vetorDisE[2] <= 25 && vetorDisE[2] >3) && (vetorDisD[0] <= 25 && vetorDisD[0] >3) && (vetorDisD[1] <= 25 && vetorDisD[1] >3) && (vetorDisD[2] <= 25 && vetorDisD[2] >3)){
        
        while((vetorDisF[0] > 25 && vetorDisF[0] >3) || (vetorDisF[1] > 25 && vetorDisF[1] >3) || (vetorDisF[2] > 25 && vetorDisF[2] >3)){
         // Serial.println("esa virando da direita pra frente os 2 tao vendo");
          direita();
          leituraDis();
          IrReceive();
          if(robotState == standby){
            loop();   
          }
        }
        frente();
      }

  }

void setup() {
  Serial.begin(115200);

  // Define os pinos do DIP switch como entrada com pull-down interno no ESP32
  for (int i = 0; i < POSITION_NUM; i++) {
    pinMode(SWITCH_PINOS[i], INPUT_PULLDOWN); // Agora usa PULLDOWN interno
  }

    pinMode(sensorInfra, INPUT);
  Serial.begin(9600);

  pinMode(PINO_ENA, OUTPUT);
  pinMode(PINO_ENB, OUTPUT);
  pinMode(PINO_IN1, OUTPUT);
  pinMode(PINO_IN2, OUTPUT);
  pinMode(PINO_IN3, OUTPUT);
  pinMode(PINO_IN4, OUTPUT);
  pinMode(sensorDisE, INPUT);
  pinMode(sensorDisD, INPUT);
  pinMode(sensorDisF, INPUT);
  digitalWrite(Standby, HIGH);
  digitalWrite(PINO_IN1, LOW); 
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, LOW);
  digitalWrite(PINO_ENA, LOW);
  digitalWrite(PINO_ENB, LOW);
  Serial.begin(115200);
}

void loop() {

  leituraSwitch();
  configPisca();
  IrReceive();
  if(robotState == ready){
    Serial.println("ENTROU NO PRIMEIRO IF");
  }
  if(robotState == fight){

    switch(valor_switch){
      case 0: //desligado - desligado - desligado

        logicaPrincipal();
      break;

      case 1: //desligado - desligado - ligado
        logicaPassiva();
      break;  

      case 2: //desligado - ligado - desligado
        logicaAtaqueLento();
      break;

      default:
      parar();
      break;
    }
  }

  else if(robotState == standby){
    parar();
    Serial.println("ENTROU NA ULTIMA FUNCAO");
  }

}
