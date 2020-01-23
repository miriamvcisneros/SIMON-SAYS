#include <LCD.h>
LCD16X2 LCD;
const int pinBuzzer = 4;


/*VARIABLES JUGADOR*/
const int t_max=600;//1000;
const int t_max_rst=200;
long t_ini=0;
const int t_max_led=200;
long t_led=0;
const int MAX_SECUENCIA=100;

long t_ini_h=0;
const int t_max_h=1000;


//pruebas
bool pres0=0;
bool pres1=0;
bool pres2=0;
bool pres3=0;

bool pulsado=0;

/*Declaracion de arrays*/
int array_secuencia[MAX_SECUENCIA];
int array_led_secuencia[MAX_SECUENCIA];
int array_jugada[MAX_SECUENCIA];

/*PULSADORES*/
const int RED_BUTTON=0; //DIGITAL0
const int BLUE_BUTTON=1; //DIGITAL1
const int GREEN_BUTTON=2;
const int YEL_BUTTON=3;
const int RESET_BUTTON=7;
/*LED ASOCIADO A LOS BOTONES*/

const int RED_LED=9;//0;
const int BLUE_LED=10;//1;
const int GREEN_LED=11;//2;
const int YEL_LED=12;//3;

/*SWITCH VARIABLES*/
int estado=0;

//ESTADOS
const int ESTADO_INI=0;
const int ESTADO_CREA_SECUENCIA=1;
const int ESTADO_ESPERA_INT=2;
const int ESTADO_COMPRUEBA=3;
const int ESTADO_FIN=4;
const int ESTADO_ESPERA_RST=7;

//N DE SECUENCIA
int N_SEC=0;//Se podra interrumpir un maximo de N_SEC veces  
int veces=0;//La variable veces va a det las veces que se ha producido int
//VELOCIDAD
int velocidad=1000;



void setup() {
  Serial.begin(9600);
  /*PINES DE LOS PULSADORES*/
  pinMode(RED_BUTTON,INPUT);
  pinMode(BLUE_BUTTON,INPUT);
  pinMode(GREEN_BUTTON,INPUT);
  pinMode(YEL_BUTTON,INPUT);
  pinMode(RESET_BUTTON,INPUT);
  /*PINES DE LOS LEDS DE LOS PULSADORES*/
  pinMode(RED_LED,OUTPUT);
  pinMode(BLUE_LED,OUTPUT);
  pinMode(GREEN_LED,OUTPUT);
  pinMode(YEL_LED,OUTPUT);
  
  randomSeed(analogRead(A5)); 
  LCD.init();
}

void loop() {
         switch(estado){
            case ESTADO_INI:
             inicio();
              veces=0;
              //Inhabilitamos las interrupciones
              detachInterrupt(0);
              detachInterrupt(1);
              detachInterrupt(2);
              detachInterrupt(3);
              attachInterrupt(digitalPinToInterrupt(7),Reset_ISR,FALLING);
              for(int i=0; i<MAX_SECUENCIA;i++){
                array_secuencia[i]=0;  
                array_jugada[i]=0;
                array_led_secuencia[i]=0;
              }
              for(int j=9; j<13;j++){
                 digitalWrite(j,LOW);  
              }
              N_SEC=1;
              velocidad=1000; //velocidad inicial
              estado=ESTADO_CREA_SECUENCIA;
              LCD.clear();
            break;
            
            case ESTADO_CREA_SECUENCIA:
              if(N_SEC==1){
                LCD.escribeLinea("INICIO",1);
              }
              else
              {
                  LCD.escribeLinea("LEVEL UP",1);
              }
              delay(200);
              crea_secuencia(N_SEC);
              estado=ESTADO_ESPERA_INT;
              velocidad=velocidad-200;
              LCD.clear();
            break;
            
            case ESTADO_ESPERA_INT:
              //Habilitamos las interrupciones
             if(pulsado==0){
              attachInterrupt(digitalPinToInterrupt(0),Boton0_ISR,FALLING);
              attachInterrupt(digitalPinToInterrupt(1),Boton1_ISR,FALLING);
              attachInterrupt(digitalPinToInterrupt(2),Boton2_ISR,FALLING);
              attachInterrupt(digitalPinToInterrupt(3),Boton3_ISR,FALLING);
             } else{
                detachInterrupt(0);
                detachInterrupt(1);
                detachInterrupt(2);
                detachInterrupt(3);
              }
              Serial.println("VECES");
              Serial.println(veces);
              if(veces==N_SEC){
                detachInterrupt(0);
                detachInterrupt(1);
                detachInterrupt(2);
                detachInterrupt(3);
                estado=ESTADO_COMPRUEBA;
              }
             break;


             
            
            case ESTADO_COMPRUEBA:
              if(comprueba()==false){
                estado=ESTADO_FIN;  
              } else {
                N_SEC=N_SEC+1;
                estado=ESTADO_CREA_SECUENCIA; 
              }
              veces=0;
            break;
            
            case ESTADO_FIN:
              LCD.escribeLinea("GAME OVER",1);
              Melodia_fin();
              LCD.clear();
              estado=ESTADO_ESPERA_RST;
            break;

            case ESTADO_ESPERA_RST:
            break;

            default:
            break;
          }
}

void crea_secuencia(int N){
  //Funcion que crea aleatoriamente una secuencia de longitud N a repetir
  int color;
  for(int i=0; i<N; i++){
    //int j=random(0,3);
    //Serial.println(j);
    color=random(9,13);
    digitalWrite(color,HIGH);
    Melodia_interrupcion();
    delay(velocidad);
    digitalWrite(color,LOW);
    delay(velocidad);
    if(color==9){
      array_secuencia[i]=0;
    }
    if(color==10){
      array_secuencia[i]=1;
    }
    if(color==11){
      array_secuencia[i]=2;
    }
    if(color==12){
      array_secuencia[i]=3;
    }
    array_led_secuencia[i]=color; //Y su led correspondiente
  }
  
  
}


void Boton0_ISR(){ 
  pulsado=1;
  delay(200);
  if(millis()-t_ini>t_max){
    if(digitalRead(0)==0){
      pres0=1;  //indica que se ha pulsado realmente
    }
    if(veces<N_SEC){
      if(pres0==1){
        t_ini=millis();
        estado=ESTADO_ESPERA_INT;
        array_jugada[veces]=RED_BUTTON;
        veces++;
        pres0=0;
      }
     }
  }  
  pulsado=0;
  delay(200);
}


void Boton1_ISR(){ 
   pulsado=1;
  delay(200); 
  if(millis()-t_ini>t_max){
    if(digitalRead(1)==0){
      pres1=1;  //indica que se ha pulsado realmente
    }
    if(veces<N_SEC){
     if(pres1==1){
      t_ini=millis();
       estado=ESTADO_ESPERA_INT;
       array_jugada[veces]=1;//AUNQUE LUEGO AQUI PONDRIAMOS LA MASCARA DE TIPO RED_BUTTON...ETC
       veces++;
       pres1=0;
     }
    }
  }  
  pulsado=0;
  delay(200);
}

void Boton2_ISR(){ 
  pulsado=1;
  delay(200);
  if(millis()-t_ini>t_max){
    if(digitalRead(2)==0){
      pres2=1;  //indica que se ha pulsado realmente
    }
    if(veces<N_SEC){
      if(pres2==1){
      t_ini=millis();
       estado=ESTADO_ESPERA_INT;
       estado=ESTADO_ESPERA_INT;
       array_jugada[veces]=2;//AUNQUE LUEGO AQUI PONDRIAMOS LA MASCARA DE TIPO RED_BUTTON...ETC
       veces++;
       pres2=0;
      } 
    }
  } 
  pulsado=0;
  delay(200);
}

void Boton3_ISR(){  
   pulsado=1;
  delay(200);
  if(millis()-t_ini>t_max){
    if(digitalRead(3)==0){
      pres3=1;  //indica que se ha pulsado realmente
    }
    if(veces<N_SEC){
      if(pres3==1){
      t_ini=millis();
      array_jugada[veces]=3;//AUNQUE LUEGO AQUI PONDRIAMOS LA MASCARA DE TIPO RED_BUTTON...ETC
      veces++;
      pres3=0;
      }
    }
  } 
  pulsado=0;
  delay(200);
}


 bool comprueba(){
  //Aqui vamos a comprobar si la secuencia introducida coincide con la secuencia generada
  bool ok=true;
  bool fin=false;
  int i=0;
  while ((ok==true )&&(fin==false)){
    if(array_secuencia[i]!=array_jugada[i]){ //Si no coincide manda un 0 y se va a ESTADO_FIN
      ok=false;  
    }
    i++;
    if(i==N_SEC){
       fin=true;
    }
  }
  return ok;
 }


 void Reset_ISR(){
  if(millis()-t_ini>t_max_rst){
    estado=ESTADO_INI;
    t_ini=millis();
  }
}

  void Melodia_interrupcion(){
  tone(pinBuzzer, 1200);
    delay(100);
    tone(pinBuzzer, 440);
    delay(100);
    tone(pinBuzzer, 523);
    delay(100);
    noTone(pinBuzzer);
  }

 void Melodia_fin(){
    tone(pinBuzzer, 500);
    delay(100);
    tone(pinBuzzer, 1200);
    delay(100);
    tone(pinBuzzer, 300);
    delay(100);
    tone(pinBuzzer, 1000);
    delay(100);
    tone(pinBuzzer, 400);
    delay(100);
    tone(pinBuzzer, 1200);
    delay(100);
    tone(pinBuzzer, 440);
    delay(100);
    tone(pinBuzzer, 523);
    delay(100);
    tone(pinBuzzer, 440);
    delay(100);
     tone(pinBuzzer, 500);
    delay(100);
    tone(pinBuzzer, 1200);
    delay(100);
    tone(pinBuzzer, 300);
    delay(100);
    tone(pinBuzzer, 1000);
    delay(100);
    tone(pinBuzzer, 400);
    delay(100);
    tone(pinBuzzer, 1200);
    delay(100);
    tone(pinBuzzer, 440);
    delay(100);
    tone(pinBuzzer, 523);
    delay(100);
    tone(pinBuzzer, 440);
    delay(100);
    noTone(pinBuzzer);
  }

    void inicio(){
    LCD.escribeLinea("WELCOME",1);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    digitalWrite(12,HIGH);

    tone(pinBuzzer, 500);
    delay(100);
    tone(pinBuzzer, 1200);
    delay(100);
    tone(pinBuzzer, 300);
    delay(100);
    tone(pinBuzzer, 1000);
    delay(100);
    tone(pinBuzzer, 400);
    delay(100);
    tone(pinBuzzer, 1200);
    delay(100);
    tone(pinBuzzer, 440);
    delay(100);
    tone(pinBuzzer, 523);
    delay(100);
    tone(pinBuzzer, 440);
    delay(100);
    tone(pinBuzzer, 500);
    delay(100);
    tone(pinBuzzer, 1200);
    delay(100);
    tone(pinBuzzer, 300);
    delay(100);
    tone(pinBuzzer, 1000);
    delay(100);
    tone(pinBuzzer, 400);
    delay(100);
    tone(pinBuzzer, 1200);
    delay(100);
    tone(pinBuzzer, 440);
    delay(100);
    tone(pinBuzzer, 523);
    delay(100);
    tone(pinBuzzer, 440);
    delay(100);
    noTone(pinBuzzer);
 
    delay(500);

    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    digitalWrite(12,LOW);

    noTone(pinBuzzer);
    LCD.clear();
  }
