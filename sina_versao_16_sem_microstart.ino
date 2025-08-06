#define DECODE_SONY
#include <IRremote.hpp>
#include "BluetoothSerial.h"
const int PINO_ENA = 33;  //pwm A
const int PINO_ENB = 13;  //pwm B
// Declaração dos pinos utilizados para controlar o sentido do motor
const int PINO_IN1 = 26;  // Primeiro motor
const int PINO_IN2 = 25;
const int PINO_IN3 = 14;  // Segundo motor
const int PINO_IN4 = 12;
const int sensorDisE = 39;  //sensores de distancia
const int sensorDisD = 35;
const int sensorDisF = 34;
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
float limiteMaiF, limiteMen, limiteMaiE, limiteMaiD;
int valAtaqueLento;
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

bool estrategiaSelecionada = false;
BluetoothSerial SerialBT; //variável para usar o bluetooth


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
    valorDisE = 13 * pow(leituraDisE *  0.00080566406, -1);
    vetorDisE[c] = valorDisE;
   /* Serial.print("vetor esquerda: ");
    Serial.println(vetorDisE[c]);*/
  }
  for (int c = 0; c < 3; c++) {
    leituraDisD = analogRead(sensorDisD);
    valorDisD = 13 * pow(leituraDisD *  0.00080566406, -1);
    vetorDisD[c] = valorDisD;
    Serial.print("vetor direita: ");
    Serial.println(vetorDisD[c]);
  }
  for (int c = 0; c < 3; c++) {
    leituraDisF = analogRead(sensorDisF);
    valorDisF = 13 * pow(leituraDisF *  0.00080566406, -1);
    vetorDisF[c] = valorDisF;
   /* Serial.print("vetor frente: ");
    Serial.println(vetorDisF[c]);*/
  }
}

void mover(char dir, int vel) {
  if (vel == 0) {
    analogWrite(PINO_ENA, 250);
    analogWrite(PINO_ENB, 200);
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
    digitalWrite(PINO_IN1, LOW);
    digitalWrite(PINO_IN2, HIGH);
    digitalWrite(PINO_IN3, LOW);
    digitalWrite(PINO_IN4, HIGH);
  } else if (dir == 'e') {  // esquerda
    digitalWrite(PINO_IN1, HIGH);
    digitalWrite(PINO_IN2, LOW);
    digitalWrite(PINO_IN3, HIGH);
    digitalWrite(PINO_IN4, LOW);
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

    while ((vetorDisF[0] > limiteMaiF || vetorDisF[1] > limiteMaiF || vetorDisF[2] > limiteMaiF) && (vetorDisE[0] > limiteMaiE || vetorDisE[1] > limiteMaiE || vetorDisE[2] > limiteMaiE) && (vetorDisD[0] >= limiteMaiD || vetorDisD[1] >= limiteMaiD || vetorDisD[2] >= limiteMaiD)) {
    SerialBT.println("não achou nada");
    mover('e', 1);

    leituraDis();
    IrReceive();
    if (robotState == standby) {
      break;
    }
  }

  if ((vetorDisF[0] <= limiteMaiF && vetorDisF[0] >= limiteMen) && (vetorDisF[1] <= limiteMaiF && vetorDisF[1] >= limiteMen) && (vetorDisF[2] <= limiteMaiF && vetorDisF[2] >= limiteMen)) {
    mover('f', 0);
    SerialBT.println("achou longe na frente");
  }

  else if ((vetorDisD[0] <= limiteMaiD && vetorDisD[0] >= limiteMen) && (vetorDisD[1] <= limiteMaiD && vetorDisD[1] >= limiteMen) && (vetorDisD[2] <= limiteMaiD && vetorDisD[2] >= limiteMen)) {
    SerialBT.println("achou longe na direita");
    while (((vetorDisF[0] > limiteMaiF && vetorDisF[0] >= limiteMen) || (vetorDisF[1] > limiteMaiF && vetorDisF[1] >= limiteMen) || (vetorDisF[2] > limiteMaiF && vetorDisF[2] >= limiteMen)) && ((vetorDisE[0] > limiteMaiE && vetorDisE[0] >= limiteMen) || (vetorDisE[1] > limiteMaiE && vetorDisE[1] >= limiteMen) || (vetorDisE[2] > limiteMaiE && vetorDisE[2] >= limiteMen))) {
      SerialBT.println("esta virando da direita pra frente");
      mover('d', 1);

      leituraDis();
      IrReceive();
      if (robotState == standby) {

        break;
      }
    }
    mover('f', 0);
    SerialBT.println("trocou da direita pra frente");
  }

  else if ((vetorDisE[0] <= limiteMaiE && vetorDisE[0] > limiteMen) && (vetorDisE[1] <= limiteMaiE && vetorDisE[1] > limiteMen) && (vetorDisE[2] <= limiteMaiE && vetorDisE[2] > limiteMen)) {
    SerialBT.println("achou longe na esquerda");                                                                                                                                              //
    while (((vetorDisF[0] > limiteMaiF && vetorDisF[0] >= limiteMen) || (vetorDisF[1] > limiteMaiF && vetorDisF[1] >= limiteMen) || (vetorDisF[2] > limiteMaiF && vetorDisF[2] >= limiteMen)) && ((vetorDisD[0] > limiteMaiD && vetorDisD[0] >=limiteMen) || (vetorDisD[1] > limiteMaiD && vetorDisD[1] >= limiteMen) || (vetorDisD[2] > limiteMaiD && vetorDisD[2] >= limiteMen))) {
      SerialBT.println("trocando da esquerda pra frente");
      mover('e', 1);
      leituraDis();
      IrReceive();
      if (robotState == standby) {
        break;
      }
    }
    mover('f', 0);
    SerialBT.println("trocou da esquerda pra frente");
  }

  /*else if ((vetorDisE[0] <= limiteMaiE && vetorDisE[0] > limiteMen) && (vetorDisE[1] <= limiteMaiE && vetorDisE[1] > limiteMen) && (vetorDisE[2] <= limiteMaiE && vetorDisE[2] > limiteMen) && (vetorDisD[0] <= limiteMaiD && vetorDisD[0] > limiteMen) && (vetorDisD[1] <= limiteMaiD && vetorDisD[1] > limiteMen) && (vetorDisD[2] <= limiteMaiD && vetorDisD[2] > limiteMen)) {

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
  }*/


}

void logicaAtaqueLento(int x) {
  mover('f', 0);
  if (cont == 0){
    if(x == 0){
   delay (500); //500
    }
    if(x == 1){
   delay (300); //300
    }

    cont++;
  }
}



void setup() {
  Serial.begin(115200);
  SerialBT.begin("esp32_BLuetooth");

  //estrategiaSelecionada = false; // Desseleciona a estratégia

  // Define os pinos do DIP switch como entrada com pull-down interno no ESP32
  for (int i = 0; i < POSITION_NUM; i++) {
    pinMode(SWITCH_PINOS[i], INPUT_PULLDOWN);  // Agora usa PULLDOWN interno
  }

  pinMode(configLed, OUTPUT);
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

bool escolherEstrategia() {
  leituraSwitch(); // Atualiza o valor_switch a partir do switch físico

  // Só continua se tiver dados disponíveis no Bluetooth

  //Serial.println(SerialBT.read());
  /*String comando = SerialBT.readStringUntil('\n'); // Recebe um comando via APP
  comando.trim(); // Remove espaços e quebras de linha
  comando.remove(0,1);
  int entrada = comando.toInt(); // Transforma em Int*/

  int entrada = SerialBT.parseInt();
  Serial.println(entrada); 


  switch (valor_switch) { // Por enquanto só recebe 0 ou 1
    case 0:
      SerialBT.println("Modo: Ajustar a distância máxima");
      if (!SerialBT.available()) return false;
      switch (entrada) {
        case 1:
          limiteMaiF = 15;
          limiteMaiE = 15;
          limiteMaiD = 15;
          SerialBT.println("Estratégia 1: Curta distância configurada.");
          estrategiaSelecionada = true;
          return estrategiaSelecionada;
    
          break;
        case 2:
          limiteMaiF = 20;
          limiteMaiE = 20;
          limiteMaiD = 20;
          SerialBT.println("Estratégia 2: Longa distância configurada.");
          estrategiaSelecionada = true;
          return estrategiaSelecionada;
          break;
        default:
          SerialBT.println("Opção inválida. Use 1 ou 2.");
          break;
      }
    break;

    case 1:
      SerialBT.println("Modo: Ataque Lento");
      if (!SerialBT.available()) return false;
      switch (entrada) {
        case 3:
          valAtaqueLento = 1;
          SerialBT.println("Estratégia 1: Ataque Lento Ativo (modo 1).");
          estrategiaSelecionada = true;
          return estrategiaSelecionada;
          break;
        case 4:
          valAtaqueLento = 0;
          SerialBT.println("Estratégia 2: Ataque Lento Ativo (modo 2).");
          estrategiaSelecionada = true;
          return estrategiaSelecionada;
          break;
        default:
          SerialBT.println("Opção inválida. Use 1 ou 2.");
          break;
      }
    break;

    default:
      Serial.println("Erro: Switch em posição inválida.");
      break;
  }

}

void loop() {
  if (!estrategiaSelecionada) {
    estrategiaSelecionada = escolherEstrategia();
  }

  //configPisca();
  IrReceive();
  if (robotState == ready) {
    /*Serial.println("ENTROU NO PRIMEIRO IF");*/
    cont = 0;
  }
  if (robotState == fight) {

    switch (valor_switch) {
      case 0:  //desligado - desligado - desligado
        //Serial.println("Lógica principal");
        /*Serial.print("Valor máximo f= ");
        Serial.println(limiteMaiF);
        Serial.print("Valor máximo e= ");
        Serial.println(limiteMaiE);
        Serial.print("Valor máximo d= ");
        Serial.println(limiteMaiD);*/
      
        logicaPrincipal();
        break;

      case 1:  //desligado - desligado - ligado
       /* Serial.println("Ataque lento");   
        Serial.print("Variavel valAtaqueLento= ");
        Serial.println(valAtaqueLento);*/

        logicaAtaqueLento(valAtaqueLento);        
        logicaPrincipal();
        break;


      default:
       Serial.println("desliga");
        mover('p', 2);
        estrategiaSelecionada = false; // Desseleciona a estratégia
        break;
    }
  }

  else if (robotState == standby) {
   /* Serial.println("Desliga");*/
    mover('p', 2);
    /*Serial.println("ENTROU NA ULTIMA FUNCAO");*/  
    }
}

