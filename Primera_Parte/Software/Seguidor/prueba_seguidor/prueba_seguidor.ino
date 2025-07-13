/* La finalidad de este programa es estudiar el seguimiento de nuestro set point, en este caso una función triangular para que el setpoint varíe con el tiempo.*/
#include <Ticker.h> /* Se incluye la librería Ticker.h */
#include "ThingSpeak.h" /* Se incluye la librería ThingSpeak.h*/
#include "WiFi.h" /* Se incluye la librería WiFi.H*/
#define N_DATA 11 // Impar

const char* ssid = "BocataDeChorizo";
const char* pasword = "DiosNoExiste";

unsigned long channelID = 1687542;
const char* WriteAPIKey = "3QUZEAC5CQG1GPIJ";

WiFiClient cliente; /* Se crea el objeto Cliente*/

const int PIN_PWM=27;
const int PIN_SEL=13;

Ticker ADC, Tick_dis; /* Lectura de la tensión en el ADC del micro */
const int PIN_ADC2 = 34;
float M_e,V_I,V_I_1,I,I_1,media,cuenta; 
float V_ref[N_DATA];

Ticker tension_celula;
const int PIN_ADC1=33;
float media_celula;
float M_e_celula;
float V_i_celula;
float V_ref_celula[N_DATA];

Ticker triangular; /* Asociado a la función triangular */
float tiempo = 20;
float t;
float pendiente = 0.0015;
float set_point = 0;
const int Dac2 = 26;
float I_MAX=700.0;

Ticker control;/* Suigue el valor del setpoint */
const int PIN_DAC = 25;  /* [0-255] Digital */
float dac ; /* Salida DAC. Entrada de la carga activa*/

void celula_fotovoltaica() {
  int i;
  int j;
  float paso_celula;
  float Aux_celula[N_DATA];
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
  V_i_celula=-0.0000000596427656610408 *M_e_celula*M_e_celula+0.004911705419677*M_e_celula-1.61866365835977;
}

void mediana() {
  float Aux[N_DATA];
  int i;
  int j;
  float paso;
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

void setpoint() {
  t += tiempo;
  set_point = pendiente * t * 1000; /* [0-700]mA SI EL SETPOINT MINIMO ES DE X VALOR, Y QUEREMOS UN PASO DE 5 SEGUNDOS, CALCULAR LA PENDINTE ES FACIL*/
  if (set_point >= I_MAX) {
    t = -tiempo;
  }
  if (set_point == 0) {
    digitalWrite(Dac2, HIGH);
  } else {
    digitalWrite(Dac2, LOW);
  }
}

void ajuste_setpoint() {
  dac = dac - (I - set_point)/600.0 * 255 * 0.1; /* Ajustar este parámetro para que la señal sea más estable I fuente, y no fluctue tanto*/
  if (dac < 0) dac = 0;
  if (dac > 255) dac = 255;
  //Serial.print(dac);
  //Serial.print("  ");
  dacWrite(PIN_DAC, dac);
}

void display_msg()
{
  //Serial.print("Tiempo: ");
  //Serial.print(cuenta);
  Serial.print("Mediana: ");
  Serial.print(M_e);
  Serial.print(" Mediana celula:");
  Serial.print(M_e_celula);
  Serial.print(" Setpoint: ");
  Serial.print(set_point);
  Serial.print(" I actual: ");
  Serial.print(I);
  Serial.print(" T celula: ");
  Serial.print(V_i_celula);
  Serial.print(" DAC actual: ");
  Serial.println(dac);
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
  pinMode(Dac2, OUTPUT);
  tension_celula.attach(0.05,celula_fotovoltaica);
  ADC.attach(0.05, mediana);
  triangular.attach(tiempo, setpoint);
  control.attach(0.05, ajuste_setpoint);
  Tick_dis.attach(1.0,display_msg);
  analogWrite(PIN_PWM,127);
}
/*Hacer otro ticker para leer los valores de ADC1 y el algoritmo de punto de máxima potencia en el loop*/
void loop() {
  /*
  delay(120000);
  ThingSpeak.setField(2,V_i_celula); // se crea el campo 
  
  ThingSpeak.writeFields(1687542,"3QUZEAC5CQG1GPIJ"); //Subimos los datos cada 2 min
  Serial.println("Datos subidos a ThingSpeak");
  */
}

/* Para N_DATA 31 no sigue bienla intensidad, fluctua mucho más. Para 21 es identica a 11 con ina indert +-3 y un tiempo de 115us a diferencia de los 95us.
 * Si usamos la media en lugar de la mediana, sigue la señal igual de bien que la mediana pero cuando setpoint=0, en lugar de valer el dac 30 vale 20.
 * Si el ticker actua cada 0,005 
 * CAMBIO ALGORITMO TRIANGULAR:
 * TMAX=50; N_SALTOS=10; T_MIN(cada salto)=TMAX/N_SALTOS = 5; I_MAX=700; I_MIN=I_MAX/N_SALTOS= 70; I=I_MAX/(T_MIN*I_MIN)*t=pendiente*t ; pendiente=I_MIN/T_MIN
*/
