long t_ini=0;

void setup() {
 Serial.begin(9600);
 pinMode(0,INPUT);
 pinMode(9,OUTPUT);
 pinMode(1,INPUT);
 pinMode(10,OUTPUT);
 pinMode(2,INPUT);
 pinMode(11,OUTPUT);
 pinMode(3,INPUT);
 pinMode(12,OUTPUT);

 //Configuracion del pin de interrupcion para flanco de bajada
 attachInterrupt(digitalPinToInterrupt(0),Boton_ISR_1,FALLING);
 attachInterrupt(digitalPinToInterrupt(1),Boton_ISR_2,FALLING);
  attachInterrupt(digitalPinToInterrupt(2),Boton_ISR_3,FALLING);
 attachInterrupt(digitalPinToInterrupt(3),Boton_ISR_4,FALLING);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void  Boton_ISR_1(){
 if(millis()-t_ini>200){
    Serial.println("Produciendose interrupcion 1");
    t_ini=millis();
    digitalWrite(9,HIGH);
  }  
  
  }

  void  Boton_ISR_2(){
 if(millis()-t_ini>200){
    Serial.println("Produciendose interrupcion 2");
    t_ini=millis();
    digitalWrite(10,HIGH);
  }  
  
  }

  void  Boton_ISR_3(){
 if(millis()-t_ini>200){
    Serial.println("Produciendose interrupcion 1");
    t_ini=millis();
    digitalWrite(11,HIGH);
  }  
  
  }

  void  Boton_ISR_4(){
 if(millis()-t_ini>200){
    Serial.println("Produciendose interrupcion 2");
    t_ini=millis();
    digitalWrite(12,HIGH);
  }  
  
  }
