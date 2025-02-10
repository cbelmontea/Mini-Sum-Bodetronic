int sensorValueF = 0, sensorValueD = 0, sensorValueE = 0;
int cmValueF = 0, cmValueE = 0, cmValueD = 0;
int sensorPinF = A6;
int sensorPinD = A7;
int sensorPinE = A5;

const int PINO_ENA = 3; //esquerda
const int PINO_ENB = 9; //direita
int sensorValorT = 0;
int sensorValorE = 0;
int sensorValorD = 0;
// Declaração dos pinos utilizados para controlar o sentido do motor
const int PINO_IN1 = 5; // Primeiro  (ESQUERDA)
const int PINO_IN2 = 4;
const int PINO_IN3 = 7; // Segundo motor
const int PINO_IN4 = 8;
const int sensorT = A2;
const int sensorE = A1;
const int sensorD = A3;




#define standby 6

void setup() {
  pinMode(PINO_ENA, OUTPUT); 
  pinMode(PINO_ENB, OUTPUT);
  pinMode(PINO_IN1, OUTPUT);
  pinMode(PINO_IN2, OUTPUT);
  pinMode(PINO_IN3, OUTPUT);
  pinMode(PINO_IN4, OUTPUT);
  pinMode(standby, OUTPUT);
  pinMode(sensorT, INPUT);
  pinMode(sensorE, INPUT);
  pinMode(sensorD, INPUT);
  digitalWrite(PINO_IN1, LOW); 
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, LOW);
  digitalWrite(PINO_ENA, LOW);
  digitalWrite(PINO_ENB, LOW);
  digitalWrite(standby, HIGH);
  pinMode(sensorPinF, INPUT);
  pinMode(sensorPinD, INPUT);
  pinMode(sensorPinE, INPUT);
  Serial.begin(9600);
  
}

void loop() {

  left();
  delay(5000);
}
void disFrente(){
  sensorValueF = analogRead(sensorPinF);
  cmValueF = 13*pow(sensorValueF * 0.0048828125 , -1);
  Serial.print("Valor lido frente= ");
  Serial.print(cmValueF);
  Serial.println(" cm");
  delay(250);
}
void disEsquerda(){    
  sensorValueE = analogRead(sensorPinE);
  cmValueE = 13*pow(sensorValueE * 0.0048828125 , -1);
  Serial.print("Valor lido esquerda= ");
  Serial.print(cmValueE);
  Serial.println(" cm");
  delay(250);
}
void disDireita(){
  sensorValueD = analogRead(sensorPinD);
  cmValueD = 13*pow(sensorValueD * 0.0048828125 , -1);
  Serial.print("Valor lido direita= ");
  Serial.print(cmValueD);
  Serial.println(" cm");
  delay(250);
}
void sensorEsquerda( ){  

  int sensorValorE = 0;
  sensorValorE = analogRead(sensorE);
  Serial.print("sensor dA ESQUERDA: ");
  Serial.println(sensorValorE);
}
void sensorTras(){

  int sensorValorT = 0;
  sensorValorT = analogRead(sensorT); 
  Serial.print("sensor de tras: ");
  Serial.println(sensorValorT);
}
void sensorDireita(){

int sensorValorD = 0;
   sensorValorD = analogRead(sensorD); 
   Serial.print("sensor da direita: ");
  Serial.println(sensorValorD);
}
void forward() {
  analogWrite(PINO_ENA, 250);
  analogWrite(PINO_ENB, 170);
  digitalWrite(PINO_IN1, HIGH); //MOTOR E
  digitalWrite(PINO_IN2, LOW); //MOTOR E
  digitalWrite(PINO_IN3, LOW); //MOTOR D
  digitalWrite(PINO_IN4, HIGH); //MOTOR D
}
void left() {
  analogWrite(PINO_ENA, 250);
  analogWrite(PINO_ENB, 170);
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);
}
void right() {
  analogWrite(PINO_ENA, 200);
  analogWrite(PINO_ENB, 170);
  digitalWrite(PINO_IN1, HIGH);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);
}
void stop() {
  analogWrite(PINO_ENA, 0);
  analogWrite(PINO_ENB, 0);
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, LOW);
}
