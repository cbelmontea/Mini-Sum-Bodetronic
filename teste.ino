#define DECODE_SONY
#include <IRremote.hpp>
#include "BluetoothSerial.h"

// --- MAPEAMENTO DOS PINOS DO ESP32 ---
const int PINO_ENA = 33;  //pwm A
const int PINO_ENB = 13;  //pwm B
const int PINO_IN1 = 26;  // Primeiro motor
const int PINO_IN2 = 25;
const int PINO_IN3 = 14;  // Segundo motor
const int PINO_IN4 = 12;
const int sensorDisE = 39;  //sensores de distancia
const int sensorDisD = 35;
const int sensorDisF = 34;
const int Standby = 27;
const int sensorInfra = 16;
const int configLed = 4;

// --- VARIÁVEIS GLOBAIS ---
float leituraDisF = 0, leituraDisE = 0, leituraDisD = 0;
float valorDisF = 0, valorDisE = 0, valorDisD = 0;
int vetorDisF[3], vetorDisE[3], vetorDisD[3];
int cont = 0;
float limiteMaiF, limiteMen, limiteMaiE, limiteMaiD;
int valAtaqueLento;
BluetoothSerial SerialBT; //variável para usar o bluetooth

// --- DEFINIÇÕES E VARIÁVEIS PARA O DIP SWITCH ---
#define POSITION_NUM 3
#define ON HIGH
#define OFF LOW
int valor_switch;
const int SWITCH_PINOS[] = { 21, 19, 18 };

// --- MÁQUINA DE ESTADOS DO ROBÔ ---
enum RobotStates {
  standby,
  ready,
  fight,
};
RobotStates robotState = standby;
bool estrategiaSelecionada = false;

// --- NOVAS VARIÁVEIS GLOBAIS PARA A LÓGICA DE FLANQUEIO ---
int estadoFlanqueio = 0; // 0=Procurando, 1=Posicionando, 2=Circulando, 3=Atacando
unsigned long tempoManobra = 0; // Para controlar o tempo das ações


void leituraSwitch() {
  valor_switch = 0;
  for (int i = 0; i < POSITION_NUM; i++) {
    int state = digitalRead(SWITCH_PINOS[i]);
    if (state == ON)
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
    valorDisE = 13 * pow(leituraDisE * 0.00080566406, -1);
    vetorDisE[c] = valorDisE;
  }
  for (int c = 0; c < 3; c++) {
    leituraDisD = analogRead(sensorDisD);
    valorDisD = 13 * pow(leituraDisD * 0.00080566406, -1);
    vetorDisD[c] = valorDisD;
  }
  for (int c = 0; c < 3; c++) {
    leituraDisF = analogRead(sensorDisF);
    valorDisF = 13 * pow(leituraDisF * 0.00080566406, -1);
    vetorDisF[c] = valorDisF;
  }
}

void mover(char dir, int vel) {
  if (vel == 0) { // Ataque
    analogWrite(PINO_ENA, 250);
    analogWrite(PINO_ENB, 200);
  } else if (vel == 1) { // Giro
    analogWrite(PINO_ENA, 220);
    analogWrite(PINO_ENB, 220);
  } else if (vel == 2) { // Parar
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

// --- NOVA FUNÇÃO AUXILIAR PARA CALCULAR A MEDIANA ---
int calcularMediana(int vetor[3]) {
  // Cria uma cópia temporária do vetor para não alterar o original
  int temp[3];
  temp[0] = vetor[0];
  temp[1] = vetor[1];
  temp[2] = vetor[2];

  // Ordena o vetor temporário (usando um método simples para 3 elementos)
  if (temp[0] > temp[1]) {
    int t = temp[0];
    temp[0] = temp[1];
    temp[1] = t;
  }
  if (temp[1] > temp[2]) {
    int t = temp[1];
    temp[1] = temp[2];
    temp[2] = t;
  }
  if (temp[0] > temp[1]) {
    int t = temp[0];
    temp[0] = temp[1];
    temp[1] = t;
  }

  // Retorna o valor do meio, que é a mediana
  return temp[1];
}

// --- LÓGICA PRINCIPAL ATUALIZADA PARA USAR A MEDIANA ---
void logicaPrincipal() {
  leituraDis(); // Faz a leitura dos 3 sensores

  // <<< MUDANÇA: Calcula a mediana de cada sensor ANTES de tomar decisões
  int medianaF = calcularMediana(vetorDisF);
  int medianaE = calcularMediana(vetorDisE);
  int medianaD = calcularMediana(vetorDisD);

  // <<< MUDANÇA: Condição de busca agora usa a mediana de cada sensor
  // Condição: ENQUANTO a mediana de todos os sensores estiver FORA do alcance...
  while (medianaF >= limiteMaiF && medianaE >= limiteMaiE && medianaD >= limiteMaiD) {
    SerialBT.println("não achou nada (mediana)");
    mover('e', 1); // Gira para a esquerda para procurar

    // Atualiza as leituras e medianas dentro do loop de busca
    leituraDis();
    medianaF = calcularMediana(vetorDisF);
    medianaE = calcularMediana(vetorDisE);
    medianaD = calcularMediana(vetorDisD);
    
    IrReceive();
    if (robotState == standby) {
      break;
    }
  }

  // <<< MUDANÇA: Condição de ataque frontal agora usa a mediana
  if (medianaF <= limiteMaiF && medianaF >= limiteMen) {
    mover('f', 0);
    SerialBT.println("achou na frente (mediana)");
  
  // <<< MUDANÇA: Condição de detecção na direita agora usa a mediana
  } else if (medianaD < limiteMaiD && medianaD >= limiteMen) {
    SerialBT.println("achou na direita (mediana)");
    
    // Gira para a direita até o oponente estar na frente
    while (calcularMediana(vetorDisF) >= limiteMaiF) { // <<< MUDANÇA: Usa a mediana aqui também
      SerialBT.println("virando da direita pra frente (mediana)");
      mover('d', 1);
      leituraDis(); // Sempre releia os sensores ao se mover
      IrReceive();
      if (robotState == standby) {
        break;
      }
    }
    mover('f', 0);
    SerialBT.println("trocou da direita pra frente (mediana)");
  
  // <<< MUDANÇA: Condição de detecção na esquerda agora usa a mediana
  } else if (medianaE < limiteMaiE && medianaE >= limiteMen) {
    SerialBT.println("achou na esquerda (mediana)");
    
    // Gira para a esquerda até o oponente estar na frente
    while (calcularMediana(vetorDisF) >= limiteMaiF) { // <<< MUDANÇA: Usa a mediana aqui também
      SerialBT.println("trocando da esquerda pra frente (mediana)");
      mover('e', 1);
      leituraDis();
      IrReceive();
      if (robotState == standby) {
        break;
      }
    }
    mover('f', 0);
    SerialBT.println("trocou da esquerda pra frente (mediana)");
  }
}

void logicaAtaqueLento(int x) {
  if (cont == 0) {
    mover('f', 0);
    if (x == 0) {
      delay(500);
    }
    if (x == 1) {
      delay(300);
    }

    // --- CORREÇÃO AQUI ---
    // 1. Para os motores para reduzir o ruído elétrico.
    mover('p', 2); 
    // 2. Dá uma pequena pausa para os sensores e a eletrônica estabilizarem.
    delay(50);     
    
    cont++;
  }
}

// --- NOVA LÓGICA DE FLANQUEIO ---
void logicaFlanquear() {
  leituraDis(); // Sempre lê os sensores primeiro

  switch (estadoFlanqueio) {
    case 0: // ESTADO 1: PROCURAR O OPONENTE
      SerialBT.println("Procurando oponente...");
      mover('e', 1); // Gira para a esquerda para procurar

      // Se qualquer sensor frontal detectar algo dentro do limite...
      if (vetorDisF[0] < limiteMaiF || vetorDisF[1] < limiteMaiF || vetorDisF[2] < limiteMaiF) {
        SerialBT.println("Oponente detectado! Posicionando para flanquear...");
        estadoFlanqueio = 1; // Muda para o estado de posicionamento
        tempoManobra = millis(); // Marca o tempo
      }
      break;

    case 1: // ESTADO 2: POSICIONAR (pequeno giro para iniciar o arco)
      mover('e', 1); // Dá um pequeno giro para a direita (para tirar o alvo da frente)
      
      // Após 300ms de giro...
      if (millis() - tempoManobra > 300) {
        SerialBT.println("Iniciando a manobra de círculo...");
        estadoFlanqueio = 2; // Muda para o estado de circular
        tempoManobra = millis(); // Reseta o timer para o timeout da manobra
      }
      break;

    case 2: // ESTADO 3: CIRCULAR (tentando manter o alvo no sensor esquerdo)
      // Move em uma curva para a direita
      moverCurva('d', 255, 100); // Roda direita rápida, esquerda devagar
      SerialBT.println("Circulando...");

      // CONDIÇÃO DE ATAQUE: Se o sensor esquerdo não vê mais o alvo...
      if (vetorDisE[0] > limiteMaiE && vetorDisE[1] > limiteMaiE) {
        SerialBT.println("Alvo perdido no flanco! Atacar!");
        estadoFlanqueio = 3; // Muda para o estado de ataque
        tempoManobra = millis(); // Marca o início do ataque
      }

      // TIMEOUT: Se ficar circulando por mais de 2,5 segundos, algo deu errado.
      if (millis() - tempoManobra > 2500) {
        SerialBT.println("Timeout! Voltando a procurar...");
        estadoFlanqueio = 0; // Volta a procurar
      }
      break;

    case 3: // ESTADO 4: ATACAR
      SerialBT.println("ATAQUE TOTAL!");
      logicaPrincipal();

      // O ataque dura 2 segundos, depois volta a procurar.
      if (millis() - tempoManobra > 2000) {
        SerialBT.println("Ataque finalizado. Procurando novo alvo...");
        estadoFlanqueio = 0; // Volta ao estado inicial
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("esp32_BLuetooth");

  for (int i = 0; i < POSITION_NUM; i++) {
    pinMode(SWITCH_PINOS[i], INPUT_PULLDOWN);
  }

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
  leituraSwitch();
  if (!SerialBT.available()) return false;
  int entrada = SerialBT.parseInt();
  SerialBT.println(entrada);
  SerialBT.println("Valor switch" + entrada);


  SerialBT.println("Escolha uma estrategia.");
  switch (valor_switch) {
    case 0: // Estratégia Principal
      SerialBT.println("Modo: Ataque Principal. Use 1 (curto) ou 2 (longo).");
      switch (entrada) {
        case 1:
          limiteMaiF = 15; limiteMaiE = 15; limiteMaiD = 15;
          SerialBT.println("Distância Curta configurada.");
          return (estrategiaSelecionada = true);
        case 2:
          limiteMaiF = 20; limiteMaiE = 20; limiteMaiD = 20;
          SerialBT.println("Distância Longa configurada.");
          return (estrategiaSelecionada = true);
      }
      break;
    case 1: // Estratégia de Ataque Lento
      SerialBT.println("Modo: Ataque Lento. Use 3 (delay 300ms) ou 4 (delay 500ms).");
       switch (entrada) {
        case 3:
          valAtaqueLento = 1;
          SerialBT.println("Ataque Lento (300ms) configurado.");
          return (estrategiaSelecionada = true);
        case 4:
          valAtaqueLento = 0;
          SerialBT.println("Ataque Lento (500ms) configurado.");
          return (estrategiaSelecionada = true);
      }
      break;
    case 2: // NOVA ESTRATÉGIA DE FLANQUEIO
      SerialBT.println("Modo: Ataque de Flanco. Use 1 (curto) ou 2 (longo).");
       switch (entrada) {
        case 1:
          limiteMaiF = 15; limiteMaiE = 15; limiteMaiD = 15;
          SerialBT.println("Distância Curta configurada para flanco.");
          return (estrategiaSelecionada = true);
        case 2:
          limiteMaiF = 25; limiteMaiE = 25; limiteMaiD = 25; // Distância maior para ter mais espaço
          SerialBT.println("Distância Longa configurada para flanco.");
          return (estrategiaSelecionada = true);
      }
      break;
    default:
      Serial.println("Erro: Switch em posição inválida.");
      break;
  }
  return false;
}

void loop() {
  if (!estrategiaSelecionada) {
    if (escolherEstrategia()) {
      SerialBT.println("Estratégia selecionada. Aguardando comando do IR.");
    }
  }

  IrReceive();
  if (robotState == ready) {
    cont = 0;
    estadoFlanqueio = 0; // Reseta o estado da lógica de flanco
  }
  if (robotState == fight) {
    switch (valor_switch) {
      case 0:  
        logicaPrincipal();
        break;
      case 1:
        logicaAtaqueLento(valAtaqueLento);
        logicaPrincipal();
        break;
      case 2: // AQUI CHAMA A NOVA ESTRATÉGIA
        logicaFlanquear();
        break;
      default:
        mover('p', 2);
        estrategiaSelecionada = false;
        break;
    }
  } else if (robotState == standby) {
    mover('p', 2);
    estadoFlanqueio = 0; // IMPORTANTE: Reseta o estado ao parar
  }
}
