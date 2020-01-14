const int pinBuzzer = 9;
void setup() {}

void loop() {
  //generar tono de 440Hz 
 tone(pinBuzzer, 440);
 delay(500);
 
 //parar tono durante 250ms  
 noTone(pinBuzzer);
 delay(250);
 
 //generar tono de 523Hz 
  tone(pinBuzzer, 523)
  delay(500);
}
