/* La finalidad de este programa es estudiar el seguimiento de nuestro set point, en este caso una función triangular para que el setpoint varíe con el tiempo.*/
#include <Ticker.h> /* Se incluye la librería Ticker.h */
#include "ThingSpeak.h" /* Se incluye la librería ThingSpeak.h*/
#include "WiFi.h" /* Se incluye la librería WiFi.H*/
#define N_DATA 11 // Impar
#define IMIN 0.0
#define IMAX 700.0
#define IINC1 100.0
#define IINC2 25.0
#define IINC3 5.0

const char* ssid = "BocataDeChorizo";
const char* pasword = "DiosNoExiste";

unsigned long channelID = 1687542;
const char* WriteAPIKey = "3QUZEAC5CQG1GPIJ";

WiFiClient cliente; /* Se crea el objeto Cliente*/

const int PIN_PWM=27; /* PWM*/
const int PIN_SEL=13; /* Relé*/

Ticker ADC, Tick_dis; // Lectura de la tensión en el ADC del micro 
const int PIN_ADC2 = 34;
float M_e,V_i,I,I_1,cuenta,media;
float V_ref[N_DATA];

Ticker tension_celula;
const int PIN_ADC1=33;
float V_ref_celula[N_DATA];
float media_celula, M_e_celula, V_celula;

Ticker control;/* Suigue el valor del setpoint */
const int PIN_DAC = 25;  /* [0-255] Digital */
float dac ; /* Salida DAC. Entrada de la carga activa*/
//------------------------------------------------------------------------------------------------------------------------------
typedef struct { /* Definimos una nueva estructura de datos similar a los registros de phyton que contiene tres variables diferentes*/
float I;
float V;
float P;  
}PMAX;

float set_point=0; /* Con esta variable se fija la Intensidad que queremos que demande la carga activa*/
float i_min;
//----------------------------------------------------------------------------------------------------------------------------------
void celula_fotovoltaica() {
  float Aux_celula[N_DATA];
  float paso_celula;
  int i,j;
  /* Realizamos la toma de datos con desplazamientos almacenando los N_DATA últimos elementos. Primero desplazamos los datos */
  for (i = 1; i < N_DATA; i++) {
    j = i - 1;
    V_ref_celula[j] = V_ref_celula[i];
  }
  V_ref_celula[N_DATA - 1] = analogReadMilliVolts(PIN_ADC1); /* Almacenamos en la última posicion el nuevo valor. */
  /* Se almacenan los datos en un vector auxiliar que posteriormente se ordenará: */
  for (i = 0; i < (N_DATA); i++) {
    Aux_celula[i] = V_ref_celula[i];
  }
  for (i = 0; i < (N_DATA - 1); i++) {
    for (j = i + 1; j < N_DATA; j++) {
      if (Aux_celula[j] < Aux_celula[i]) {
        paso_celula = Aux_celula[j];
        Aux_celula[j] = Aux_celula[i];
        Aux_celula[i] = paso_celula;
      }
    }
  }
  float suma=0;
  for (i = 0; i<(N_DATA); i++) {
    suma+=Aux_celula[i];
  }
  media_celula=suma/N_DATA;
  M_e_celula = Aux_celula[N_DATA / 2];
  V_celula=-0.0000000596427656610408 *M_e_celula*M_e_celula+0.004911705419677*M_e_celula-1.61866365835977;
}

void intensidad() {
  float Aux[N_DATA];
  float paso;
  int i,j;
  /* Realizamos la toma de datos con desplazamientos almacenando los N_DATA últimos elementos. Primero desplazamos los datos */
  long ti,tf;
  ti = micros();
  for (i = 1; i < N_DATA; i++) {
    j = i - 1;
    V_ref[j] = V_ref[i];
  }
  V_ref[N_DATA - 1] = analogReadMilliVolts(PIN_ADC2); /* Almacenamos en la última posicion el nuevo valor. */
  /* Se almacenan los datos en un vector auxiliar que posteriormente se ordenará: */
  for (i = 0; i < (N_DATA); i++) {
    Aux[i] = V_ref[i];
  }
  for (i = 0; i < (N_DATA - 1); i++) {
    for (j = i + 1; j < N_DATA; j++) {
      if (Aux[j] < Aux[i]) {
        paso = Aux[j];
        Aux[j] = Aux[i];
        Aux[i] = paso;
      }
    }
  }
  float suma=0;
  for (i = 0; i<(N_DATA); i++) {
    suma+=Aux[i];
  }
  media=suma/N_DATA;
  M_e = Aux[N_DATA / 2]; /* Se obtiene el valor de la mediana y se calcula la tensión en V_i de la salida de la carga activa.*/
  I= (-0.00000000586780571462522 * M_e * M_e + 0.000493468763266 * M_e - 0.160901604616591)*1000; /* Ajuste cuadrático en mA*/
  //V_i = ((M_e / 1000 - 12080 * 9880 * 3.2 / (46770.0 * (12080.0 + 9880.0) + (9880.0 * 12080.0))) * (46770.0 * (12080.0 + 9880.0) + (9880.0 * 12080.0)) / (9880.0 * 46770.0)); /* en Voltios */
  //V_i_1 = ((media / 1000 - 12080 * 9880 * 3.3 / (46770.0 * (12080.0 + 9880.0) + (9880.0 * 12080.0))) * (46770.0 * (12080.0 + 9880.0) + (9880.0 * 12080.0)) / (9880.0 * 46770.0)); /* en Voltios */
  //I = V_i/5.0 * 1000; /* mA */
  //I_1=V_i_1/5.0*1000;
  tf = micros();
  cuenta=tf-ti;
}

void ajuste_setpoint() {
  dac = dac - (I - set_point)/600.0 * 255 * 0.1; /* Ajustar este parámetro para que la señal sea más estable I fuente, y no fluctue tanto*/
  if (dac < 0) dac = 0;
  if (dac > 255) dac = 255;
  dacWrite(PIN_DAC, dac);
}

//-------------------------------------------------------------------------------------------------------------------------------
PMAX busca_max(float mini,float maxi,float inc){
  PMAX ret;
  float P_max, P_aux;
  for (float k=mini; k <= maxi ; k=k+inc){ 
    set_point=k;
    while ((I-set_point) > 3){
      Serial.print(" 1_W ");
      Serial.print(" Set_point: ");
      Serial.print(set_point);
      Serial.print(" I: ");
      Serial.println(I);
      delay(50);//0,05 Segundos
      if (set_point >= maxi){
      delay(200);
      break;
      }
    }
    P_max=I/1000.0*V_celula;
    Serial.print(" P_max: ");
    Serial.print(P_max);
    if (P_max > P_aux){
      Serial.print("Hi");;
      while ((I-set_point) > 2){
        delay(50);
        if (set_point >= maxi){
          break;
        }
      }
      ret.I=I/1000.0;
      ret.V=V_celula;
      ret.P=ret.I*ret.V;
    } else{
      break;
      }
   P_aux=P_max;
  }
  return ret;
}

//------------------------------------------------------------------------------------------------------------------------------------
void display_msg(){
  //Serial.print("Tiempo: ");
  //Serial.print(cuenta);
  //Serial.print("Mediana: ");
  //Serial.print(M_e);
  //Serial.print(" Mediana celula:");
  //Serial.print(M_e_celula);
  //Serial.print(" I actual: ");
  //Serial.print(I);
  //Serial.print(" T celula: ");
  //Serial.print(V_celula);
  //Serial.print(" DAC actual: ");
  //Serial.println(dac);
}

void setup() {
  Serial.begin(115200);
  //WiFi.begin("BocataDeChorizo","DiosNoExiste"); /* Nos conectamos al router WiFi*/
  //while (WiFi.status() != WL_CONNECTED){
  //  delay(500);
  //  Serial.print(".");   
  //}
  //Serial.println("Wifi conectado!");
  //ThingSpeak.begin(cliente); /* Inicializamos servidor ThingSpeak */
  
  analogReadResolution(11);
  analogSetAttenuation(ADC_6db);
  pinMode(PIN_SEL,OUTPUT);
  digitalWrite(PIN_SEL,LOW);/* Entrada izq=3.3=fijo, Drch=0=movil*/
  pinMode(PIN_PWM,OUTPUT);
  tension_celula.attach(0.05,celula_fotovoltaica);
  ADC.attach(0.05, intensidad);
  control.attach(0.05, ajuste_setpoint);
  //Tick_dis.attach(1.0,display_msg);
  analogWrite(PIN_PWM,127);
}

void loop() {
  //------------------------------------------------------------------------------------------------------------------
  PMAX ret;
  digitalWrite(PIN_SEL,LOW);
  delay(100);
  ret=busca_max(IMIN,IMAX,IINC1);
  //i_min=ret.I;
  //ret=busca_max(i_min,i_min+IINC1,IINC2);
  //i_min=ret.I;
  //ret=busca_max(i_min,i_min+IINC2,IINC3);
  //ThingSpeak.setField(1,ret.I); // Se crea el campo
  //ThingSpeak.setField(2,ret.V); 
  //ThingSpeak.setField(3,ret.P);
  Serial.print(" Int_MAX: ");
  Serial.print(ret.I);
  Serial.print(" Vol_max: ");
  Serial.print(ret.V);
  Serial.print(" Pot_max: ");
  Serial.println(ret.P);
  //digitalWrite(PIN_SEL,HIGH);
  //ret=busca_max(IMIN,IMAX,IINC1);
  //i_min=ret.I;
  //ret=busca_max(i_min,i_min+IINC1,IINC2);
  //i_min=ret.I;
  //ret=busca_max(i_min,i_min+IINC2,IINC3);
  //P2=ret;
  //Serial.print(" I_MAX: ");
  //ThingSpeak.setField(4,ret.I); 
  //ThingSpeak.setField(5,ret.V); 
  //ThingSpeak.setField(6,ret.P);
  //ThingSpeak.writeFields(1687542,"3QUZEAC5CQG1GPIJ"); //Subimos los datos cada 2 min
  //Serial.println("Datos subidos a ThingSpeak");
  delay(6000);
  //-------------------------------------------------------------------------------------------------------------------
}

/*
 Este código se puede simplificar más, haciendo una función que calcule la mediana.
 Además, se puede hacer una función que suba los datos a internet. Para esto necesito PMAX P1,P2; y esos campos son los que entran a la nueva función
 para posteriormente ser subidos a la nube.
 Si quiero una programación modular:
 -función mediana();
 -funcion tension();
 -funcion intensidad();
 -funcion ajuste_setpoint();
 -funcion busca_max();
 -funcion nube();
*/

/* Para N_DATA 31 no sigue bienla intensidad, fluctua mucho más. Para 21 es identica a 11 con ina indert +-3 y un tiempo de 115us a diferencia de los 95us.
 * Si usamos la media en lugar de la mediana, sigue la señal igual de bien que la mediana pero cuando setpoint=0, en lugar de valer el dac 30 vale 20.
 * Si el ticker actua cada 0,005, correcto.
*/
