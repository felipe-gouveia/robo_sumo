 #define borda A0 //definimos que no pino A0 será conectado o sensor de borda  
 #define trigger 13 //definimos que o pino 13 funcionará como trigger do sensor ultrasônico  
 #define echo 8 //definimos o pino 8 como echo do sensor ultrasônico  
 #define MOTORDIR01 4 //definimos que a porta 5 será o bit 1 da ponte H do motor direito (0x)  
 #define MOTORDIR02 5 //definimos que a porta 6 será o bit 2 da ponte H do motor direito (x0)  
 #define MOTORESQ01 6 //definimos que a porta 3 será o bit 1 da ponte H do motor esquerdo(0x)  
 #define MOTORESQ02 7 //definimos que a porta 4 será o bit 1 da ponte H do motor esquerdo(x0)  

 int valorSensorFrente = 0;
 int valorPreto;
 
 //a ponte H funciona da seguinte maneira: (MOTORDIR01,MOTORDIR02,MOTORESQ01,MOTORESQ02)  
 // (0,1,0,1) Anda pra frente  
 // (1,0,1,0) Anda pra trás  
 // (0,0,0,0) Parado  
 
 /******************** Biblioteca de Time ***************************/  
 typedef struct Timer  
 {  
   unsigned long start;//Armazena o tempo de quando foi iniciado o timer  
   unsigned long timeout;//Tempo após o start para o estouro  
 };  
 unsigned long Now ( void )  
 {  
   return millis ( );//Retorna os milisegundos depois do reset  
 }  
 boolean TimerEstorou (struct Timer * timer)  
 {  
   //Verifica se o timer estourou  
   if ( Now () > timer->start + timer->timeout) {  
     return true;  
   }  
   return false;   
 }
   
 //após o tempo estourar temos que iniciar o timer com o tempo atual  
 void timerStart(struct Timer * timer){  
   timer->start = Now();  
 }
   
 void timerDesloc(struct Timer * timer, unsigned long tempo){  
  timer->start = Now()+tempo;  
 }  
 
 Timer timerBorda = {0, 10}; //Verifica se achou a borda a cada 10 milisegundos  
 Timer timerAchou = {0, 70}; //verifica se achou o oponente a cada 70 milisegundos  
 Timer timerFrente = {0, 3243}; //tempo que o robô passa indo pra frente, 3243  
 Timer timerGira = {0, 1600}; //tempo que o robô passa girando
 Timer timerfinal = {0, 85000};// tempo de 1:30 minutos, final do round  
 
 boolean sentidoGiro = true; //variável criada para o robô girar hora pra direita, hora pra esquerda  
 
 /********** Função preparatória ****************/  
void setup(){  
  
  Serial.begin(9600);
  
  // OUTPUT  
  pinMode(MOTORDIR01, OUTPUT);  
  pinMode(MOTORDIR02, OUTPUT);  
  pinMode(MOTORESQ01, OUTPUT);  
  pinMode(MOTORESQ02, OUTPUT);  
  pinMode(trigger, OUTPUT);  
  
  // INPUT  
  pinMode(borda, INPUT);  
  pinMode(echo, INPUT);  
  
  parado();  
  calibra();
  
  Serial.print("Ref:");
  Serial.println(valorPreto);
  
  
  delay(5000); //espera de 5 segundos antes do início da partida. 
 
  timerFrente.start= Now() - timerFrente.timeout; // Robô começar a luta indo pra frente.  
  timerGira.start= Now();  
  timerAchou.start= Now();  
}
   
 int i=0;  
 
void loop(){  
 
 if(TimerEstorou(&timerfinal)){  
  while(1)  
   parado(); 
 }  
 
 if(i=0)  
 {  
  timerStart(&timerBorda);  
  timerStart(&timerGira);  
  timerStart(&timerAchou);  
  i+=1;  
 }  
 
 if(TimerEstorou(& timerFrente)){  
  andaFrente();  
  timerStart(& timerFrente);  
 }
   
 if(TimerEstorou(& timerBorda)){  
  if(linha()){// verifica se achou a linha 
   andaTras();  
   delay(1000);
   timerStart(& timerFrente);  
   timerDesloc(& timerGira, -timerGira.timeout);  
   timerStart(& timerAchou);  
  }  
  timerStart(& timerBorda);  
 }
  
 if(TimerEstorou(& timerAchou)){  
  if(achou()){  
   andaFrente();  
   timerStart(& timerGira);  
  }  
  timerStart(& timerAchou);  
 }
   
 if(TimerEstorou(& timerGira)){  
   
   Serial.println(valorSensorFrente);
   
  if(sentidoGiro){  
   giraDireita();  
   sentidoGiro=!sentidoGiro;  
  }  
  else  
  {  
   giraEsquerda();  
   sentidoGiro=!sentidoGiro;  
  }  
  timerStart(& timerGira);  
 }  
}  
 
 /********** Movimentação do Robô ****************/  
 void andaTras(){ 
  digitalWrite(MOTORDIR01, HIGH);  
  digitalWrite(MOTORDIR02, LOW);  
  digitalWrite(MOTORESQ01, HIGH);  
  digitalWrite(MOTORESQ02, LOW);  
 }
   
 void andaFrente(){  
  digitalWrite(MOTORDIR01, LOW);  
  digitalWrite(MOTORDIR02, HIGH);  
  digitalWrite(MOTORESQ01, LOW);  
  digitalWrite(MOTORESQ02, HIGH);  
 }
   
 void parado(){  
  digitalWrite(MOTORDIR01, LOW);  
  digitalWrite(MOTORDIR02, LOW);  
  digitalWrite(MOTORESQ01, LOW);  
  digitalWrite(MOTORESQ02, LOW);  
 }
   
 void giraEsquerda(){  
  digitalWrite(MOTORDIR01, HIGH);  
  digitalWrite(MOTORDIR02, LOW);  
  digitalWrite(MOTORESQ01, LOW);  
  digitalWrite(MOTORESQ02, HIGH);  
 }  
 
 void giraDireita(){
  digitalWrite(MOTORDIR01, LOW);  
  digitalWrite(MOTORDIR02, HIGH);  
  digitalWrite(MOTORESQ01, HIGH);  
  digitalWrite(MOTORESQ02, LOW);  
 }
   
 /********** Função de busca e calibragem ****************/  
 float distancia() {  
  float tempo, dist;  
  digitalWrite(trigger, LOW);  
  delayMicroseconds(2);  
  digitalWrite(trigger, HIGH);  
  delayMicroseconds(10);  
  tempo = pulseIn(echo, HIGH);  
  dist = (((tempo/2) * 350)/1000000);  
  dist *= 100;  
  return dist;  
 }  

 void calibra(){
  valorSensorFrente = analogRead(borda);
    valorPreto = (valorSensorFrente);  
 }

 boolean linha(){
  valorSensorFrente = analogRead(borda);
  if((valorSensorFrente < valorPreto - 125))
    return true;
  else
    return false;
 }
 boolean achou(){  
  if (distancia() < 75)  
   return true;  
  else  
   return false;  
 }  
