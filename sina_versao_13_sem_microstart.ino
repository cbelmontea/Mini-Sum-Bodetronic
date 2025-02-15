#define DECODE_SONY
#include <IRremote.hpp>

const int PINO_ENA = 33;  //pwm A
const int PINO_ENB = 13;  //pwm B
// Declaração dos pinos utilizados para controlar o sentido do motor
const int PINO_IN1 = 26;  // Primeiro motor
const int PINO_IN2 = 25;
const int PINO_IN3 = 14;  // Segundo motor
const int PINO_IN4 = 12;
const int sensorDisE = 39;  //sensores de distancia
const int sensorDisD = 34;
const int sensorDisF = 35;
float leituraDisF = 0;  //valor da leitura dos sensores de distância
float leituraDisE = 0;
float leituraDisD = 0;
float valorDisF = 0;  //valor da leitura dos sensores de distância convertido em cm
float valorDisE = 0;
float valorDisD = 0;
int vetorDisF[3], vetorDisE[3], vetorDisD[3];
const int Standby = 27;
int cont = 0;
int val;
float limiteMaiF = 3, limiteMen = -1, limiteMaiE = 1.65, limiteMaiD = 3.30;

unsigned long tempoParada = 0;
const unsigned long intervaloLeitura = 100;  // intervalo de 1 segundo
unsigned long tempoAnterior = 0;
int estado = 0;
const int sensorInfra = 16;
const int configLed = 4;

#define POSITION_NUM 3  // Alterado para 3 chaves
#define ON HIGH         // Agora o estado ativo é HIGH
#define OFF LOW         // Estado inativo é LOW
int valor_switch;
// Define os pinos conectados ao DIP switch
const int SWITCH_PINOS[] = { 21, 19, 18 };

unsigned long blinkTimer;
bool ledOn;
decode_results results;

enum RobotStates {
  standby,
  ready,
  fight,
};

RobotStates robotState = standby;


void configPisca() {
  switch (valor_switch) {
    case 0:  //desligado - desligado - desligado
      digitalWrite(configLed, HIGH);
      delay(1000);
      digitalWrite(configLed, LOW);
      delay(1000);
      break;

    case 1:  //desligado - desligado - ligado
      digitalWrite(configLed, HIGH);
      delay(1000);
      digitalWrite(configLed, LOW);
      delay(1000);
      digitalWrite(configLed, HIGH);
      delay(1000);
      digitalWrite(configLed, LOW);
      delay(1000);
      break;

    case 2:  //desligado - ligado - desligado
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

void leituraSwitch() {
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

void leituraDis() {
  for (int c = 0; c < 3; c++) {
    leituraDisE = analogRead(sensorDisE);
    valorDisE = 13 * pow(leituraDisE * 0.0048828125, -1);
    vetorDisE[c] = valorDisE;
    //vetorDisE[c] = 15;
    Serial.print("vetor esquerda: ");
    Serial.println(vetorDisE[c]);
  }
  for (int c = 0; c < 3; c++) {
    leituraDisD = analogRead(sensorDisD);
    valorDisD = 13 * pow(leituraDisD * 0.0048828125, -1);
    vetorDisD[c] = valorDisD;
    Serial.print("vetor direita: ");
    Serial.println(vetorDisD[c]);
  }
  for (int c = 0; c < 3; c++) {
    leituraDisF = analogRead(sensorDisF);
    valorDisF = 13 * pow(leituraDisF * 0.0048828125, -1);
    vetorDisF[c] = valorDisF;
    Serial.print("vetor frente: ");
    Serial.println(vetorDisF[c]);
  }
}

void mover(char dir, int vel) {
  if (vel == 0) {
    analogWrite(PINO_ENA, 255);
    analogWrite(PINO_ENB, 255);
  } else if (vel == 1) {
    analogWrite(PINO_ENA, 220);
    analogWrite(PINO_ENB, 220);
  } else if (vel == 2) {
    analogWrite(PINO_ENA, 0);
    analogWrite(PINO_ENB, 0);
  }
  if (dir == 'f') {  // frente
    digitalWrite(PINO_IN1, LOW);
    digitalWrite(PINO_IN2, HIGH);
    digitalWrite(PINO_IN3, HIGH);
    digitalWrite(PINO_IN4, LOW);
  } else if (dir == 'd') {  // direita
    digitalWrite(PINO_IN1, HIGH);
    digitalWrite(PINO_IN2, LOW);
    digitalWrite(PINO_IN3, HIGH);
    digitalWrite(PINO_IN4, LOW);
  } else if (dir == 'e') {  // esquerda
    digitalWrite(PINO_IN1, LOW);
    digitalWrite(PINO_IN2, HIGH);
    digitalWrite(PINO_IN3, LOW);
    digitalWrite(PINO_IN4, HIGH);
  } else if (dir == 'p') {  // parar
    digitalWrite(PINO_IN1, LOW);
    digitalWrite(PINO_IN2, LOW);
    digitalWrite(PINO_IN3, LOW);
    digitalWrite(PINO_IN4, LOW);
  }
}

void buscaCarol() {
  if (tempoParada == 0) {
    tempoParada = millis();
  }
  analogWrite(PINO_ENA, 200);
  analogWrite(PINO_ENB, 200);
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);
  if (millis() - tempoAnterior > intervaloLeitura) {  // verifica se já passou o intervalo desde a última leitura
    mover('e', 1);
  }
}


void logicaPrincipal() {
  leituraDis();

    while ((vetorDisF[0] > limiteMaiF || vetorDisF[1] > limiteMaiF || vetorDisF[2] > limiteMaiF) && (vetorDisE[0] > limiteMaiE || vetorDisE[1] > limiteMaiE || vetorDisE[2] > limiteMaiE) && (vetorDisD[0] > limiteMaiD || vetorDisD[1] > limiteMaiD || vetorDisD[2] > limiteMaiD)) {
    //Serial.println("não achou nada");
    mover('e', 1);

    leituraDis();
    IrReceive();
    if (robotState == standby) {
      break;
    }
  }

  if ((vetorDisF[0] <= limiteMaiF && vetorDisF[0] > limiteMen) && (vetorDisF[1] <= limiteMaiF && vetorDisF[1] > limiteMen) && (vetorDisF[2] <= limiteMaiF && vetorDisF[2] > limiteMen)) {
    mover('f', 0);
    //Serial.println("achou longe da frente");
  }

  else if ((vetorDisD[0] <= limiteMaiD && vetorDisD[0] > limiteMen) && (vetorDisD[1] <= limiteMaiD && vetorDisD[1] > limiteMen) && (vetorDisD[2] <= limiteMaiD && vetorDisD[2] > limiteMen)) {
    Serial.println("achou longe na direita");
    while ((vetorDisF[0] > limiteMaiF && vetorDisF[0] > limiteMen) || (vetorDisF[1] > limiteMaiF && vetorDisF[1] > limiteMen) || (vetorDisF[2] > limiteMaiF && vetorDisF[2] > limiteMen)) {
      //Serial.println("esta virando da direita pra frente");
      mover('d', 1);

      leituraDis();
      IrReceive();
      if (robotState == standby) {
        break;
      }
    }
    mover('f', 0);
    //Serial.println("trocou da direita pra frente");
  }

  else if ((vetorDisE[0] <= limiteMaiE && vetorDisE[0] > limiteMen) && (vetorDisE[1] <= limiteMaiE && vetorDisE[1] > limiteMen) && (vetorDisE[2] <= limiteMaiE && vetorDisE[2] > limiteMen)) {
    //Serial.println("achou longe na esquerda");
    while ((vetorDisF[0] > limiteMaiF && vetorDisF[0] > limiteMen) || (vetorDisF[1] > limiteMaiF && vetorDisF[1] > limiteMen) || (vetorDisF[2] > limiteMaiF && vetorDisF[2] > limiteMen)) {
      //Serial.println("trocando da esquerda pra frente");
      mover('e', 1);
      leituraDis();
      IrReceive();
      if (robotState == standby) {
        break;
      }
    }
    mover('f', 0);
    //Serial.println("trocou da esquerda pra frente");
  }

  else if ((vetorDisE[0] <= limiteMaiE && vetorDisE[0] > limiteMen) && (vetorDisE[1] <= limiteMaiE && vetorDisE[1] > limiteMen) && (vetorDisE[2] <= limiteMaiE && vetorDisE[2] > limiteMen) && (vetorDisD[0] <= limiteMaiD && vetorDisD[0] > limiteMen) && (vetorDisD[1] <= limiteMaiD && vetorDisD[1] > limiteMen) && (vetorDisD[2] <= limiteMaiD && vetorDisD[2] > limiteMen)) {

    while ((vetorDisF[0] > limiteMaiF && vetorDisF[0] > limiteMen) || (vetorDisF[1] > limiteMaiF && vetorDisF[1] > limiteMen) || (vetorDisF[2] > limiteMaiF && vetorDisF[2] > limiteMen)) {
      //Serial.println("esa virando da direita pra frente os 2 tao vendo");
      mover('d', 1);
      leituraDis();
      IrReceive();
      if (robotState == standby) {
        break;
      }
    }
    mover('f', 0);
  }


}

void logicaAtaqueLento(int x) {
  mover('f', 0);
  if (cont == 0){
    if(x == 0){
   delay (500);
    }
    if(x == 1){
   delay (300);
    }


    cont++;
  }
}

void setup() {
  Serial.begin(115200);

  // Define os pinos do DIP switch como entrada com pull-down interno no ESP32
  for (int i = 0; i < POSITION_NUM; i++) {
    pinMode(SWITCH_PINOS[i], INPUT_PULLDOWN);  // Agora usa PULLDOWN interno
  }

  pinMode(configLed, INPUT);
  IrReceiver.begin(sensorInfra);


  pinMode(PINO_ENA, OUTPUT);
  pinMode(PINO_ENB, OUTPUT);
  pinMode(PINO_IN1, OUTPUT);
  pinMode(PINO_IN2, OUTPUT);
  pinMode(PINO_IN3, OUTPUT);
  pinMode(PINO_IN4, OUTPUT);
  pinMode(sensorDisE, INPUT);
  pinMode(sensorDisD, INPUT);
  pinMode(sensorDisF, INPUT);
  pinMode(Standby, OUTPUT);
  digitalWrite(Standby, HIGH);
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, LOW);
  digitalWrite(PINO_ENA, LOW);
  digitalWrite(PINO_ENB, LOW);
}

void loop() {

  leituraSwitch();
  //configPisca();
  IrReceive();
  if (robotState == ready) {
    Serial.println("ENTROU NO PRIMEIRO IF");
    cont = 0;
  }
  if (robotState == fight) {

    switch (valor_switch) {
      case 3:  //desligado - desligado - desligado
        //Serial.println("Lógica principal");
        limiteMaiF = 7, limiteMaiE = 1.30, limiteMaiD = 3;
        logicaPrincipal();
        break;

      case 5:  //desligado - desligado - ligado
        //Serial.println("Lógica passiva");
        limiteMaiF = 4, limiteMaiE = 0.65, limiteMaiD = 1.5;
        logicaPrincipal();
        break;

      case 6:  //desligado - ligado - desligado
        //Serial.println("Ataque lento");
        limiteMaiF = 7, limiteMaiE = 1.30, limiteMaiD = 3;      
        logicaAtaqueLento(0);
        logicaPrincipal();
        break;
      
      case 1:
        limiteMaiF = 7, limiteMaiE = 1.30, limiteMaiD = 3;
        logicaAtaqueLento(1);
        logicaPrincipal();

      break;

      default:
        Serial.println("desliga");
        mover('p', 2);
        break;
    }
  }

  else if (robotState == standby) {
    Serial.println("Desliga");
    mover('p', 2);
    Serial.println("ENTROU NA ULTIMA FUNCAO");
    }
}
