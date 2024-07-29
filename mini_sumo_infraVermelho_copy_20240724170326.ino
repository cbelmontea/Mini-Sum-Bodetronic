#include <IRremote.h>
const int PINO_ENA = 5; //pwm A
const int PINO_ENB = 11; //pwm B
// Declaração dos pinos utilizados para controlar o sentido do motor
const int PINO_IN1 = 6; // Primeiro motor
const int PINO_IN2 = 7;
const int PINO_IN3 = 9; // Segundo motor
const int PINO_IN4 = 10;
const int sensorInfraT = A1; //sensores infra-vermelho
const int sensorInfraE = A0;
const int sensorInfraD = A2;
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
#define standby 8
int cont = 0;
//const int IRreceiver = 3;
//unsigned long valor_remoto;
int vetorDisF[3], vetorDisE[3], vetorDisD[3];
const int sensorInfra = 3;
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
 /* //Serial.println("busca");
  analogWrite(PINO_ENA, 40);
  analogWrite(PINO_ENB, 40);
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);

  leituraDisF = analogRead(sensorDisF);
  leituraDisE = analogRead(sensorDisE);
  leituraDisD = analogRead(sensorDisD);

  valorDisF = 13*pow(leituraDisF * 0.0048828125 , -1);
  valorDisE = 13*pow(leituraDisE * 0.0048828125 , -1);
  valorDisD = 13*pow(leituraDisD * 0.0048828125 , -1);
*/

  //delay(5000);
  /*analogWrite(PINO_ENA, 20);
  analogWrite(PINO_ENB, 20);
  digitalWrite(PINO_IN1, HIGH);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);*/
  
  //while(valorDisF > 25 && valorDisE > 25 && valorDisD > 25){*/
  analogWrite(PINO_ENA, 150);
  analogWrite(PINO_ENB, 150);
  digitalWrite(PINO_IN1, HIGH);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);
   /* leituraDisF = analogRead(sensorDisF);
    leituraDisE = analogRead(sensorDisE);
    leituraDisD = analogRead(sensorDisD);

    valorDisF = 13*pow(leituraDisF * 0.0048828125 , -1);
    valorDisE = 13*pow(leituraDisE * 0.0048828125 , -1);
    valorDisD = 13*pow(leituraDisD * 0.0048828125 , -1);*/

  
}
void re(){
 // Serial.println("re");
  analogWrite(PINO_ENA, 217);
  analogWrite(PINO_ENB, 150);
  digitalWrite(PINO_IN1, HIGH);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);  
}
void frente() {
//  Serial.println("frente");
  analogWrite(PINO_ENA, 217);
  analogWrite(PINO_ENB, 150);
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);
}
void esquerda() {
 // Serial.println("esquerda");
  analogWrite(PINO_ENA, 150);
  analogWrite(PINO_ENB, 150);
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);
}

void direita() {
  //Serial.println("direita");
  analogWrite(PINO_ENA, 150);
  analogWrite(PINO_ENB, 150);
  digitalWrite(PINO_IN1, HIGH);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);
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
  analogWrite(PINO_ENA, 150);
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
     //loop2();
  mediaT = leituraIfT();
  mediaD = leituraIfD();
  mediaE = leituraIfE();
  Serial.print("leitura de tras: ");
  Serial.println(mediaT);
    Serial.print("leitura da direita: ");
  Serial.println(mediaD);
    Serial.print("leitura da esquerda: ");
  Serial.println(mediaE);
    for(int c = 0; c < 3; c++){
      leituraDisE = analogRead(sensorDisE);
      valorDisE = 13*pow(leituraDisE * 0.0048828125 , -1);
      vetorDisE[c] = valorDisE;
      //Serial.print("vetor esquerda: ");
      //Serial.println(vetorDisE[c]);
    }
    for(int c = 0; c < 3; c++){
      leituraDisD = analogRead(sensorDisD);
      valorDisD = 13*pow(leituraDisD * 0.0048828125 , -1); 
      vetorDisD[c] = valorDisD; 
      //Serial.print("vetor direita: ");
      //Serial.println(vetorDisD[c]);      
    }
    for(int c = 0; c < 3; c++){
      leituraDisF = analogRead(sensorDisF);
      valorDisF = 13*pow(leituraDisF * 0.0048828125 , -1);
      vetorDisF[c] = valorDisF;
      //Serial.print("vetor frente: ");
      //Serial.println(vetorDisF[c]);      
    }
  
  /*Serial.println("distancia da frente: ");
  Serial.println(valorDisF);

  Serial.println("distancia da esquerda: ");
  Serial.println(valorDisE);

  Serial.println("distancia da direita: ");
  Serial.println(valorDisD);*/

    if(mediaT >= 680 && mediaE >= 650  && mediaD >= 650){ // Avalia se ta no preto
      
      while((vetorDisF[0] > 30 || vetorDisF[1] > 30 || vetorDisF[2] > 30) && (vetorDisE[0] > 30 || vetorDisE[1] > 30 || vetorDisE[2] > 30) && (vetorDisD[0] > 30 || vetorDisD[1] > 30 || vetorDisD[2] > 30)){
        Serial.println("não achou nada");
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

          if(mediaT < 680 || mediaE < 650 || mediaD < 650){
            Serial.println("entrou no goto de procura");
            goto infraNoBranco;  
          }      
      }   

      if(vetorDisF[0] <= 30 && vetorDisF[1] <= 30 && vetorDisF[2] <= 30){
        frente();        
        Serial.println("achou longe da frente");
      }

      else if(vetorDisD[0] <= 30 && vetorDisD[1] <= 30 && vetorDisD[2] <= 30){
        Serial.println("achou longe na direita");
        
        while(vetorDisF[0] > 30 || vetorDisF[1] > 30 || vetorDisF[2] > 30){
          Serial.println("esta virando da direita pra frente");
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

          if(mediaT < 680 || mediaE < 650  || mediaD < 650){
            Serial.println("entrou no goto de virar da direita pra frente");
            goto infraNoBranco;
              
          }    
        }              
        frente();        
       Serial.println("trocou da direita pra frente");        
      }

      else if(vetorDisE[0] <= 30 && vetorDisE[1] <= 30 && vetorDisE[2] <= 30){
        Serial.println("achou longe na esquerda");
        while(vetorDisF[0] > 30 || vetorDisF[1] > 30 || vetorDisF[2] > 30){
          Serial.println("trocando da esquerda pra frente");
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

          if(mediaT < 680 || mediaE < 650 || mediaD < 650){
            Serial.println("entrou no goto de virar da esquerda pra frente");
            goto infraNoBranco;  
          }    
        }
        frente();         
        Serial.println("trocou da esquerda pra frente");
      }

      else if(vetorDisE[0] <= 30 && vetorDisE[1] <= 30 && vetorDisE[2] <= 30 && vetorDisD[0] <= 30 && vetorDisD[1] <= 30 && vetorDisD[2] <= 30){
        
        while(vetorDisF[0] > 30 || vetorDisF[1] > 30 || vetorDisF[2] > 30){
          Serial.println("esa virando da direita pra frente");
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

          if(mediaT < 680 || mediaE < 650  || mediaD < 650){
            Serial.println("entrou no goto de virar da direita pra frente");
            goto infraNoBranco;
              
          }    
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
    }

    else{
      infraNoBranco:
       Serial.println("nao reconheceu o preto");
      // Criar funções pra, caso ele identifique o branco consiga voltar pro preto OK
      if(mediaT < 680 && mediaE >= 650 && mediaD >= 650){ // Se ler o branco pelo sensor infra de tras
        frente();    
        Serial.println("fugiu da borda pelo de tras"); 
        delay(900);
       
      }
      else if(mediaE < 650 && mediaD >= 650 && mediaT >= 680){ // Se ler o branco pelo sensor infra da esquerda
        parar();
        direita();
        Serial.println("fugiu da borda pelo da esquerda");
        delay(900);       
        frente(); // Tira o frente????
          
      }
      else if(mediaD < 650 && mediaE >= 650 && mediaT >= 680){ // Se ler o branco pelo sensor infra da direita
        parar();
        esquerda();
        Serial.println("fugiu da borda pelo da direita");
        delay(900);
        frente(); // Tira o frente????
       
      }
      else if(mediaD < 650 && mediaE < 650 && mediaT >= 680){
        parar();
        re();
        Serial.println("fugiu da borda pelos das laterais"); 
        delay(900); 
      }
      else if(mediaE < 650 && mediaT < 680 && mediaD >= 650){
        direita();
        delay(500);
        frente();
      }
      else if(mediaD < 650 && mediaT < 680 && mediaE >= 650){
        esquerda();
        delay(500);
        frente();
      }
      else if(mediaE < 650 && mediaD < 650 && mediaT < 680){
        re();
        delay(500);
      }      
    }
      }
  /*else if (val == 0){
    parar();
    val = digitalRead(sensorInfra)+ 
    1;
    
  }*/
//}
else if(val == 0){
  parar();
}
}
