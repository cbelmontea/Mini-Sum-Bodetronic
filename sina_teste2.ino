int sensorValue = 0;
int cmValue = 0;
int dir = 1;  //define direção do sensor ou do motor que será testado (1-frente, 2-direita, 3-esquerda)

// SENSORES DE DISTANCIA
int sensorPinE = 34;
int sensorPinD = 35;
int sensorPinF = 39;
// PINOS PONTE H
const int PINO_IN1 = 26;
const int PINO_IN2 = 25;
const int PINO_IN3 = 14;
const int PINO_IN4 = 12;
// PINOS PWM
const int PWMA = 33;
const int PWMB = 13;
#define STBY 27

void setup() {
  Serial.begin(115200);
  pinMode(PINO_IN1, OUTPUT);
  pinMode(PINO_IN2, OUTPUT);
  pinMode(PINO_IN3, OUTPUT);
  pinMode(PINO_IN4, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(sensorPinE, INPUT);
  pinMode(sensorPinD, INPUT);
  pinMode(sensorPinF, INPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
}

void sensorDis(int dir) {
  int sensorPin;

  if (dir == 0) {
    sensorPin = sensorPinF;
    Serial.print("Valor lido frente = ");
  } else if (dir == 1) {
    sensorPin = sensorPinD;
    Serial.print("Valor lido direita = ");
  } else {
    sensorPin = sensorPinE;
    Serial.print("Valor lido esquerda = ");
  }

  sensorValue = analogRead(sensorPin);
  cmValue = 13 * pow(sensorValue * 0.0048828125, -1);

  Serial.print(cmValue);
  Serial.println(" cm");
  delay(250);
}

void mover(int dir) {
  Serial.println("movendo");
  analogWrite(PWMA, 200);
  analogWrite(PWMB, 200);
  if (dir == 0) {  //frente
    digitalWrite(PINO_IN1, LOW);
    digitalWrite(PINO_IN2, HIGH);
    digitalWrite(PINO_IN3, HIGH);
    digitalWrite(PINO_IN4, LOW);
  } else if (dir == 1) {  // direita
    digitalWrite(PINO_IN1, HIGH);
    digitalWrite(PINO_IN2, LOW);
    digitalWrite(PINO_IN3, HIGH);
    digitalWrite(PINO_IN4, LOW);
  } else if (dir == 2) {  // esquerda
    digitalWrite(PINO_IN1, LOW);
    digitalWrite(PINO_IN2, HIGH);
    digitalWrite(PINO_IN3, LOW);
    digitalWrite(PINO_IN4, HIGH);
  }
}

void loop() {
  /*
  for (dir = 0; dir < 3; dir++) { //testa sensor em todas as direções, se for usar descomentar o "for" e comentar linha 83
    sensorDis(dir);  //
  } */
  sensorDis(dir); //testa resposta do sensor na direção desejada
  mover(dir); //move na direção desejada
}
