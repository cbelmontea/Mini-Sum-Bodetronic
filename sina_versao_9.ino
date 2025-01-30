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
float leituraDisF = 0; //valor da leitura dos sensores de distância
float leituraDisE = 0; 
float leituraDisD = 0;
float valorDisF = 0; //valor da leitura dos sensores de distância convertido em cm
float valorDisE = 0;
float valorDisD = 0;
float leituraInfraT = 0; //valor da leitura dos sensores infra-vermelho
float leituraInfraE = 0;
float leituraInfraD = 0;
float somaT, mediaT;
float somaE, mediaE;
float somaD, mediaD;
#define standby 6
int cont = 0;
int val;
float limiteMai = 15, limiteMen = 3;
float vetorDisF[3], vetorDisE[3], vetorDisD[3];
const int sensorInfra = 11;

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

float leituraIfE(){
  leituraInfraE = analogRead(sensorInfraE);
  somaE = 0, mediaE = 0;
    for(int c = 0; c<6 ; c++){
      somaE= somaE + leituraInfraE;
    }
  mediaE = somaE/6;
  
return mediaE;
}

float leituraIfD(){
  leituraInfraD = analogRead(sensorInfraD);
  somaD = 0, mediaD = 0;
   for(int c = 0; c<6 ; c++){
     somaD= somaD + leituraInfraD;
   }
  mediaD = somaD/6;
  
  return mediaD;
}

float leituraIfT(){
  leituraInfraT = analogRead(sensorInfraT);
  somaT = 0, mediaT = 0;      
    for(int c = 0; c<6 ; c++){
     somaT= somaT + leituraInfraT;
    }
  mediaT = somaT/6;

return mediaT;     
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


void setup () {
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
  digitalWrite(standby, HIGH);
  digitalWrite(PINO_IN1, LOW); 
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, LOW);
  digitalWrite(PINO_ENA, LOW);
  digitalWrite(PINO_ENB, LOW);
  Serial.begin(9600);
}

void loop () {
  val = digitalRead(sensorInfra);
  Serial.println(val);
  parar:
  if(val == 1){
    Serial.println("ligou");

  leituraDis();  
          
      while((vetorDisF[0] > limiteMai || vetorDisF[1] > limiteMai || vetorDisF[2] > limiteMai) && (vetorDisE[0] > limiteMai || vetorDisE[1] > limiteMai || vetorDisE[2] > limiteMai) && (vetorDisD[0] > limiteMai || vetorDisD[1] > limiteMai || vetorDisD[2] > limiteMai)){
        Serial.println("não achou nada");
        busca();       
        mediaT = leituraIfT();          
        mediaD = leituraIfD();
        mediaE = leituraIfE();

        leituraDis();  
        val = digitalRead(sensorInfra);
        Serial.println(val);
          if(val != 1){
            goto parar;  
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
          mediaT = leituraIfT();          
          mediaD = leituraIfD();
          mediaE = leituraIfE();

          leituraDis();   
        val = digitalRead(sensorInfra);
        Serial.println(val);
          if(val != 1){
            goto parar;  
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
          mediaT = leituraIfT();
          mediaD = leituraIfD();
          mediaE = leituraIfE();

          leituraDis(); 
          leituraDis();  
          val = digitalRead(sensorInfra);
          Serial.println(val);
            if(val != 1){
              goto parar;  
            }     
        }
        frente();         
        Serial.println("trocou da esquerda pra frente");
      }

      else if((vetorDisE[0] <= limiteMai && vetorDisE[0] >limiteMen) && (vetorDisE[1] <= limiteMai && vetorDisE[1] >limiteMen) && (vetorDisE[2] <= limiteMai && vetorDisE[2] >limiteMen) && (vetorDisD[0] <= limiteMai && vetorDisD[0] >limiteMen) && (vetorDisD[1] <= limiteMai && vetorDisD[1] >limiteMen) && (vetorDisD[2] <= limiteMai && vetorDisD[2] >limiteMen)){
        
        while((vetorDisF[0] >limiteMai && vetorDisF[0] >limiteMen) || (vetorDisF[1] > limiteMai && vetorDisF[1] >limiteMen) || (vetorDisF[2] > limiteMai && vetorDisF[2] >limiteMen)){
          Serial.println("esa virando da direita pra frente os 2 tao vendo");
          direita();
          mediaT = leituraIfT();          
          mediaD = leituraIfD();
          mediaE = leituraIfE();

          leituraDis();
        leituraDis();  
        val = digitalRead(sensorInfra);
        Serial.println(val);
          if(val != 1){
            goto parar;  
          }   
        }
        frente();
      }
  }
  else if(val == 0){
    Serial.println("desligou");
    parar();
  }

}
