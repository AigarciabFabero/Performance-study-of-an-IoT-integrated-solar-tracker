#include <Ticker.h> /* Incluimos la librería Ticker.h */

Ticker setpoint;
float tiempo=0.5;
float pendiente=0.5;
float set_point=0;
float t;
const int Dac1=25;
const int Dac2=26;

void triangular(){
  t+= tiempo;
  set_point=int(pendiente*t*255/3.3);
  Serial.println(t);
  Serial.println(set_point);
  if (set_point >= 232){
    t=-tiempo;
    Serial.print(" t: ");
    Serial.println(t);
  }
  if (set_point == 0){
      digitalWrite(Dac2,HIGH);
  } else {
      digitalWrite(Dac2,LOW);
    }
  dacWrite(Dac1,set_point);
}
  
void setup() {
  Serial.begin(115200);
  pinMode(Dac1,OUTPUT);
  pinMode(Dac2,OUTPUT);
  setpoint.attach(tiempo,triangular);
}

void loop() {
  delay(500);
}
//Codigo del seguidor para la triangular. Parte inferior.
/*
Ticker triangular;  Asociado a la función triangular 
float tiempo = 20;
float t;
float pendiente = 0.0015;
float set_point = 0;
const int Dac2 = 26;
float I_MAX=700.0;
 * 
  void setpoint() {
  t += tiempo;
  set_point = pendiente * t * 1000;  [0-700]mA SI EL SETPOINT MINIMO ES DE X VALOR, Y QUEREMOS UN PASO DE 5 SEGUNDOS, CALCULAR LA PENDINTE ES FACIL
  if (set_point >= I_MAX) {
    t = -tiempo;
  }
  if (set_point == 0) {
    digitalWrite(Dac2, HIGH);
  } else {
    digitalWrite(Dac2, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(11);
  analogSetAttenuation(ADC_6db);
  pinMode(Dac2, OUTPUT);
 */
