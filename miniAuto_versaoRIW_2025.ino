

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
int estadoRobo = 0;
int entrada2;
int contTeste = 0;
int entrada = 0;
int val;
float limiteMaiF, limiteMen, limiteMaiE, limiteMaiD;
int valAtaqueLento;
unsigned long tempoParada = 0;
const unsigned long intervaloLeitura = 100;  // intervalo de 1 segundo
unsigned long tempoAnterior = 0;
int estado = 0;
const int sensorInfra = 16;
const int configLed = 4;

int roboSelecionado = 0; // 0 - Nenhum, 1 - Mikasa, 2 - Sina

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

// --- NOVAS VARIÁVEIS GLOBAIS PARA A LÓGICA DE FLANQUEIO ---
int estadoFlanqueio = 0; // 0=Procurando, 1=Posicionando, 2=Circulando, 3=Atacando
unsigned long tempoManobra = 0; // Para controlar o tempo das ações

// --- NOVA FUNÇÃO PARA MOVIMENTO EM CURVA ---
void moverCurva(char dir, int velEsquerda, int velDireita) {
  // Define a direção para FRENTE para ambos os motores
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);
  
  // Controla a velocidade de cada motor individualmente para fazer a curva
  if (dir == 'd') { // Curva para a direita
    analogWrite(PINO_ENA, velEsquerda); // Roda externa (esquerda) mais rápida
    analogWrite(PINO_ENB, velDireita);  // Roda interna (direita) mais lenta
  } else if (dir == 'e') { // Curva para a esquerda
    analogWrite(PINO_ENA, velDireita);   // Roda interna (esquerda) mais lenta
    analogWrite(PINO_ENB, velEsquerda);  // Roda externa (direita) mais rápida
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

void mover(int a, char dir, int vel) {
  switch(a){
    case 8: // Código Mikasa
      if (vel == 0) {
        analogWrite(PINO_ENA, 250);
        analogWrite(PINO_ENB, 230);
      } else if (vel == 1) {
        analogWrite(PINO_ENA, 200);
        analogWrite(PINO_ENB, 150);
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
      break;
    case 9: // Código Sina
      if (vel == 0) {
        analogWrite(PINO_ENA, 250);
        analogWrite(PINO_ENB, 200);
      } else if (vel == 1) {
        analogWrite(PINO_ENA, 150);
        analogWrite(PINO_ENB, 150);
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
    break;
  }
}

// --- NOVA LÓGICA DE FLANQUEIO ---
void logicaFlanquear() {
  leituraDis(); // Sempre lê os sensores primeiro

    SerialBT.println(estadoFlanqueio);

  switch (estadoFlanqueio) {
    case 0: // ESTADO 1: PROCURAR O OPONENTE
      //SerialBT.println("Procurando oponente...");
      mover(roboSelecionado, 'e', 1); // Gira para a esquerda para procurar

      // Se qualquer sensor frontal detectar algo dentro do limite...
      if (vetorDisF[0] <= limiteMaiF || vetorDisF[1] <= limiteMaiF || vetorDisF[2] <= limiteMaiF) {
        //SerialBT.println("Oponente detectado! Posicionando para flanquear...");
        estadoFlanqueio = 1; // Muda para o estado de posicionamento
        tempoManobra = millis(); // Marca o tempo
      }
      break;

    case 1: // ESTADO 2: POSICIONAR (pequeno giro para iniciar o arco)
      mover(roboSelecionado,'d', 1); // Dá um pequeno giro para a direita (para tirar o alvo da frente)
      
      // Após 300ms de giro...
      if (millis() - tempoManobra > 250) {  //era millis() - tempoManobra > 300
        //SerialBT.println("Iniciando a manobra de círculo...");
        estadoFlanqueio = 2; // Muda para o estado de circular
        tempoManobra = millis(); // Reseta o timer para o timeout da manobra
      }
      break;

    case 2: // ESTADO 3: CIRCULAR (tentando manter o alvo no sensor esquerdo)
      // Move em uma curva para a direita
      moverCurva('e', 200, 100); // Roda direita rápida, esquerda devagar
      //SerialBT.println("Circulando...");

      // CONDIÇÃO DE ATAQUE: Se o sensor esquerdo não vê mais o alvo...
      if (vetorDisE[0] > limiteMaiE && vetorDisE[1] > limiteMaiE) {
        //SerialBT.println("Alvo perdido no flanco! Atacar!");
        estadoFlanqueio = 3; // Muda para o estado de ataque
        tempoManobra = millis(); // Marca o início do ataque
      }

      // TIMEOUT: Se ficar circulando por mais de 2,5 segundos, algo deu errado.
      if (millis() - tempoManobra > 2500) {
        //SerialBT.println("Timeout! Voltando a procurar...");
        estadoFlanqueio = 0; // Volta a procurar
      }
      break;

    case 3: // ESTADO 4: ATACAR
      //SerialBT.println("ATAQUE TOTAL!");
      logicaPrincipal();

      // O ataque dura 2 segundos, depois volta a procurar.
      if (millis() - tempoManobra > 2000) {
        //SerialBT.println("Ataque finalizado. Procurando novo alvo...");
        estadoFlanqueio = 0; // Volta ao estado inicial
      }
      break;
  }
}

void logicaPrincipal() {
  leituraDis();

    while ((vetorDisF[0] > limiteMaiF || vetorDisF[1] > limiteMaiF || vetorDisF[2] > limiteMaiF) && (vetorDisE[0] > limiteMaiE || vetorDisE[1] > limiteMaiE || vetorDisE[2] > limiteMaiE) && (vetorDisD[0] >= limiteMaiD || vetorDisD[1] >= limiteMaiD || vetorDisD[2] >= limiteMaiD)) {
    SerialBT.println("não achou nada");
    mover(roboSelecionado,'e', 1);

    leituraDis();
    IrReceive();
    if (robotState == standby) {
      break;
    }
  }

  if ((vetorDisF[0] <= limiteMaiF && vetorDisF[0] >= limiteMen) && (vetorDisF[1] <= limiteMaiF && vetorDisF[1] >= limiteMen) && (vetorDisF[2] <= limiteMaiF && vetorDisF[2] >= limiteMen)) {
    mover(roboSelecionado,'f', 0);
    SerialBT.println("achou longe na frente");
  }

  else if ((vetorDisD[0] <= limiteMaiD && vetorDisD[0] >= limiteMen) && (vetorDisD[1] <= limiteMaiD && vetorDisD[1] >= limiteMen) && (vetorDisD[2] <= limiteMaiD && vetorDisD[2] >= limiteMen)) {
    SerialBT.println("achou longe na direita");
    while (((vetorDisF[0] > limiteMaiF && vetorDisF[0] >= limiteMen) || (vetorDisF[1] > limiteMaiF && vetorDisF[1] >= limiteMen) || (vetorDisF[2] > limiteMaiF && vetorDisF[2] >= limiteMen)) && ((vetorDisE[0] > limiteMaiE && vetorDisE[0] >= limiteMen) || (vetorDisE[1] > limiteMaiE && vetorDisE[1] >= limiteMen) || (vetorDisE[2] > limiteMaiE && vetorDisE[2] >= limiteMen))) {
      SerialBT.println("esta virando da direita pra frente");
      mover(roboSelecionado,'d', 1);

      leituraDis();
      IrReceive();
      if (robotState == standby) {

        break;
      }
    }
    mover(roboSelecionado,'f', 0);
    SerialBT.println("trocou da direita pra frente");
  }

  else if ((vetorDisE[0] <= limiteMaiE && vetorDisE[0] > limiteMen) && (vetorDisE[1] <= limiteMaiE && vetorDisE[1] > limiteMen) && (vetorDisE[2] <= limiteMaiE && vetorDisE[2] > limiteMen)) {
    SerialBT.println("achou longe na esquerda");                                                                                                                                              //
    while (((vetorDisF[0] > limiteMaiF && vetorDisF[0] >= limiteMen) || (vetorDisF[1] > limiteMaiF && vetorDisF[1] >= limiteMen) || (vetorDisF[2] > limiteMaiF && vetorDisF[2] >= limiteMen)) && ((vetorDisD[0] > limiteMaiD && vetorDisD[0] >=limiteMen) || (vetorDisD[1] > limiteMaiD && vetorDisD[1] >= limiteMen) || (vetorDisD[2] > limiteMaiD && vetorDisD[2] >= limiteMen))) {
      SerialBT.println("trocando da esquerda pra frente");
      mover(roboSelecionado,'e', 1);
      leituraDis();
      IrReceive();
      if (robotState == standby) {
        break;
      }
    }

    mover(roboSelecionado,'f', 0);
    SerialBT.println("trocou da esquerda pra frente");
  }
}

void logicaAtaqueLento(int x) {
  mover(roboSelecionado,'f', 0);
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
  SerialBT.begin("miniAutoBodetronic");

  roboSelecionado = 0;
  estadoRobo = 0;

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

  while(contTeste == 0){
    SerialBT.println("Escolha um Robô");
    entrada = SerialBT.parseInt();
    SerialBT.println(entrada);
    
      if(entrada == 8){
        SerialBT.println("Robo Selecionado: Mikasa");
        roboSelecionado = 8;
        entrada = 0;
        estadoRobo = 1;
        contTeste = 1;

      }else if(entrada == 9){
        SerialBT.println("Robô selecionado: Sina");
        roboSelecionado = 9;
        entrada = 0;
        estadoRobo = 1;
        contTeste = 1;
      } 
  }

  if (!SerialBT.available()){
    return false;  // Só continua se tiver dados disponíveis no Bluetooth
  }

  if(entrada == 0){
    while(SerialBT.available()){
      SerialBT.read();
    }
  }

  switch(estadoRobo){
    case 0: // Selecionar estadoRobo      

/*      if(entrada == 8){
        SerialBT.println("Robo Selecionado: Mikasa");
        roboSelecionado = 8;
        entrada = 0;
        estadoRobo = 1;

      }else if(entrada == 9){
        SerialBT.println("Robô selecionado: Sina");
        roboSelecionado = 9;
        entrada = 0;
        estadoRobo = 1;
      }*/
    
    break;

    case 1:
      leituraSwitch(); // Atualiza o valor_switch a partir do switch físico

      switch (valor_switch) { // recebe 0, 1 ou 2
          case 0:
            SerialBT.println("Modo: Ajustar a distância máxima");
            entrada2 = SerialBT.parseInt();
            SerialBT.println(entrada2);
            switch (entrada2) {
              case 1:
                limiteMaiF = 15;
                limiteMaiE = 15;
                limiteMaiD = 15;
                SerialBT.println("Estratégia 1: Curta distância configurada.");
                estadoRobo = 2;
                estrategiaSelecionada = true;
                return estrategiaSelecionada;          
              break;

              case 2:
                limiteMaiF = 35;
                limiteMaiE = 30;
                limiteMaiD = 30;
                SerialBT.println("Estratégia 2: Longa distância configurada.");
                estadoRobo = 2;
                estrategiaSelecionada = true;
                return estrategiaSelecionada;
              break;
              default:
              SerialBT.println("estrategia incorreta");
            }
        break;

        case 1:
          SerialBT.println("Modo: Ataque Lento");
          entrada2 = SerialBT.parseInt();
          switch (entrada2) {
            case 3:
              valAtaqueLento = 1;
              limiteMaiF = 35;
              limiteMaiE = 30;
              limiteMaiD = 30;
              SerialBT.println("Estratégia 1: Ataque Lento Ativo (modo 1).");
              estadoRobo = 2;
              estrategiaSelecionada = true;
              return estrategiaSelecionada;
              break;
            case 4:
              valAtaqueLento = 0;
              limiteMaiF = 35;
              limiteMaiE = 30;
              limiteMaiD = 30;
              SerialBT.println("Estratégia 2: Ataque Lento Ativo (modo 2).");
              estadoRobo = 2;
              estrategiaSelecionada = true;
              return estrategiaSelecionada;
              break;
              default:
              SerialBT.println("estrategia incorreta");
          }
        break;

        case 2: // NOVA ESTRATÉGIA DE FLANQUEIO
          SerialBT.println("Modo: Ataque de Flanco. Use 1 (curto) ou 2 (longo).");
          entrada2 = SerialBT.parseInt();
          switch (entrada2) {
            case 1:
              limiteMaiF = 15; limiteMaiE = 15; limiteMaiD = 15;
              SerialBT.println("Distância Curta configurada para flanco.");
              estadoRobo = 2;
              return (estrategiaSelecionada = true);
            break;
            case 2:
              limiteMaiF = 25; limiteMaiE = 25; limiteMaiD = 25; // Distância maior para ter mais espaço
              SerialBT.println("Distância Longa configurada para flanco.");
              estadoRobo = 2;
              return (estrategiaSelecionada = true);
            break;
            default:
            SerialBT.println("estrategia incorreta");
            }            
        break;
      }
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
    cont = 0;
  }
  if (robotState == fight) {

    switch (valor_switch) {
      case 0:  //desligado - desligado - desligado      
        logicaPrincipal();
        break;

      case 1:  //desligado - desligado - ligado

        logicaAtaqueLento(valAtaqueLento);        
        logicaPrincipal();
        break;

      case 2: // desligado - ligado - desligado
        logicaFlanquear();
       break;

      default:
        SerialBT.println("desliga");
        mover(roboSelecionado,'p', 2);
        estrategiaSelecionada = false; // Desseleciona a estratégia
        break;
    }
  }

  else if (robotState == standby) {
    mover(roboSelecionado,'p', 2);
    estadoFlanqueio = 0; // IMPORTANTE: Reseta o estado ao parar
    }
}
