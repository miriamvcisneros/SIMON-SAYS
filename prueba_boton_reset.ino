const int t_max=200;
long t_ini=0;
void setup() {

  Serial.begin(9600);
  pinMode(7,INPUT);

  //Configuracion del pin de interrupcion para flanco de subida a bajada
    attachInterrupt(digitalPinToInterrupt(7),Boton_ISR,FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:

}
void Boton_ISR(){   
  if(millis()-t_ini>t_max){
    Serial.println("Produciendose interrupcion");
    t_ini=millis();
    //digitalWrite(1,HIGH);
  }
}
