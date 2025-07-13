
/* La finalidad de este programa es estudiar el seguimiento de nuestro set point, en este caso una función triangular para que el setpoint varíe con el tiempo.*/

#include <Arduino.h>
#include <Ticker.h> /* Se incluye la librería Ticker.h */
#include "ThingSpeak.h" /* Se incluye la librería ThingSpeak.h*/
#include "WiFi.h" /* Se incluye la librería WiFi.H*/
#include <PID_v1.h> 
#include "ESP32Time.h" //ESP32Time.h es una biblioteca de tiempos que hace una sincronización con el servidor NTP.
#include "WiFi.h" //WiFi.h es la libreria wifi para conectar el microcontrolador a la red wifi.
#include "UBTech.h" //UBTech.h es una librería que se encarga del control de los servomotores.

#define TESTING   false // Modo de prueba para ver la respuesta a escalones

// Constantes de muestreo
#define N_DATA 5 //11 // Impar

// Constantes y estructura de la función busca_max
#define IMIN 0.0
#define IMAX 700.0 //700.0
#define IINC 100.0
#define MININC 0.75    //Salto mínimo de corriente al buscar (mA)
#define BUSCA_MAX_TIMEOUT 2000   // Tiempo (ms) que busca_max espera para que se alcance el setpoint
#define TEST_PPS  false           // Para hacer la busqueda más lenta cuando se usa la fuente

typedef struct { /* Definimos una nueva estructura de datos similar a los registros de phyton que contiene tres variables diferentes*/
  float I;
  float V;       // Tensión en el punto de máxima potencia
  float P;      // Potencia máxima
  float I_last; // Ultimo valor en el que se pudo fijar el setpoint
  int status; // Indica si el resultado es fiable: 0=OK, -1=Timeout
} PMAX;

// Constantes lazo de control
#define N_DATA_ERR  11 //21 // Número de datos del histórico del error al buscar 
#define USE_PID 0

// Constantes referidas a los Tickers
#define CTRL_TIME 0.005  // Tiempo de ciclo (s) del lazo de control
#define SAMPLE_TIME 0.001 // Tiempo de muestreo
#define DISP_TIME 0.05

const int PIN_PWM = 27; /* PWM*/
const int PIN_SEL = 13; /* Relé*/
const int PIN_ADC2 = 34;	// Corriente
const int PIN_ADC1 = 33;	// Tensión
const int PIN_DAC = 25;  /* [0-255] Digital */

typedef struct {
  float media;
  float mediana;
  float desviacion;
  long tiempo;
} ARRSTAT;

unsigned long channelID = 1687542;
const char* WriteAPIKey = "3QUZEAC5CQG1GPIJ";

WiFiClient cliente; /* Se crea el objeto Cliente*/

Ticker tick_corriente, tick_disp; // Lectura de la tensión en el ADC del micro
float M_e, V_i, I_1, media;
double I;
float V_ref[N_DATA];

Ticker tick_tension;
float V_ref_celula[N_DATA];
float media_celula, M_e_celula, V_celula;

Ticker tick_control;/* Sigue el valor del setpoint */
double dac ; /* Salida DAC. Entrada de la carga activa*/

float err_hist[N_DATA_ERR];   // Array con el historico del error
bool err_fix;                   // Indica si el control ha 'enganchado'
ARRSTAT err_stat;

//Define the aggressive and conservative Tuning Parameters
//double aggKp = 4, aggKi = 0.2, aggKd = 1;
//double consKp = 1, consKi = 0.05, consKd = 0.25;

double aggKp = 4, aggKi = 0.2, aggKd = 1;
//double consKp = 0.25, consKi =1.2, consKd = 0.00;

double consKp = 0.25, consKi =1.0, consKd = 0.0;

double set_point = 0; /* Con esta variable se fija la Intensidad que queremos que demande la carga activa*/

PID pid(&I, &dac, &set_point, consKp, consKi, consKd, P_ON_M, DIRECT);

float i_min;

long ti, tf, tiempo;

//--------------------------------------------------------//
//---------------RELATIVO AL SEGUIDOR SOLAR---------------//
//--------------------------------------------------------//

#define PI 3.14159265358979324 //Número PI
#define Longitud 38.886 //En grados 
#define Latitud -7.007 //En grados. Postitivo al Este del Meridiano_G y negativo al Oeste

//Nombre y contraseña del WiFi:
const char* ssid = "APTest1";
const char* password = "isoTech25";

const char* ntpServer = "pool.ntp.org"; //Servidor NTP
const long gmtOffset_sec = 0; //Desplazamiento GMT * 3600, España +2 pero empleamos el UTC para evitar el horario administrativo.
const int daylightOffset_sec = 0; //Compensación Diurna

typedef struct {
  float Azimutal;
  float Altitud;
}ANGULO;

float Local_Time,Elevacion,Acimutal;
int Year, Day_of_Year, hora;

ESP32Time rtc; //Creamos el objeto objeto

//relativo al movimiento de los servos:
SoftwareSerial ss14(14, 14, false);
UBTech servo(&ss14, &Serial);  // Debug on Serial
int servoCnt = 0;

//----------------------------------------------------------------------------------------------------------------------------------

/*******************************************************************************************
   Función para introducir un nuevo valor y calcular la estadística de un array
*/

ARRSTAT insert_calcula_stat_array(float *data, int ndat, float newdat)
{
  ARRSTAT ret;
  float aux_data[ndat];
  float paso_data;
  int i, j;
  long t_i, t_f;

  t_i = micros();
  /* Realizamos la toma de datos con desplazamientos almacenando los N_DATA últimos elementos. Primero desplazamos los datos */
  for (i = 1; i < ndat; i++) {
    j = i - 1;
    data[j] = data[i];
  }
  data[ndat - 1] = newdat; /* Almacenamos en la última posicion el nuevo valor. */
  /* Se almacenan los datos en un vector auxiliar que posteriormente se ordenará: */
  for (i = 0; i < (ndat); i++) {
    aux_data[i] = data[i];
  }
  for (i = 0; i < (ndat - 1); i++) {
    for (j = i + 1; j < ndat; j++) {
      if (aux_data[j] < aux_data[i]) {
        paso_data = aux_data[j];
        aux_data[j] = aux_data[i];
        aux_data[i] = paso_data;
      }
    }
  }
  float suma = 0;
  for (i = 0; i < (ndat); i++) {
    suma += aux_data[i];
  }
  ret.media = suma / ndat;
  ret.mediana = aux_data[ndat / 2];

  suma = 0;
  for (i = 0; i < (ndat); i++) {
    suma += (aux_data[i] - ret.media) * (aux_data[i] - ret.media);
  }
  ret.desviacion = sqrt(suma / ndat);
  t_f = micros();
  ret.tiempo = t_f - t_i;

  return ret;
}

/******************************************************************************
  Función para muestrear la tensión
*/

void tension() {

  ARRSTAT valores;
  float newdata;

  newdata =analogReadMilliVolts(PIN_ADC1); /* Almacenamos en la última posicion el nuevo valor. */
  valores = insert_calcula_stat_array(V_ref_celula, N_DATA, newdata);

  media_celula = valores.media;
  M_e_celula = valores.mediana;
  V_celula = -0.0000000596427656610408 * M_e_celula * M_e_celula + 0.004911705419677 * M_e_celula - 1.61866365835977;
}

/******************************************************************************
  Función para muestrear la corriente
*/

void intensidad() {

  ARRSTAT valores;
  float newdata;

  ti = micros();
  newdata =analogReadMilliVolts(PIN_ADC2); /* Almacenamos en la última posicion el nuevo valor. */
  valores = insert_calcula_stat_array(V_ref_celula, N_DATA, newdata);

  media = valores.media;
  M_e = valores.mediana;
  I = (-0.00000000586780571462522 * M_e * M_e + 0.000493468763266 * M_e - 0.160901604616591) * 1000; /* Ajuste cuadrático en mA*/
  //V_i = ((M_e / 1000 - 12080 * 9880 * 3.2 / (46770.0 * (12080.0 + 9880.0) + (9880.0 * 12080.0))) * (46770.0 * (12080.0 + 9880.0) + (9880.0 * 12080.0)) / (9880.0 * 46770.0)); /* en Voltios */
  //V_i_1 = ((media / 1000 - 12080 * 9880 * 3.3 / (46770.0 * (12080.0 + 9880.0) + (9880.0 * 12080.0))) * (46770.0 * (12080.0 + 9880.0) + (9880.0 * 12080.0)) / (9880.0 * 46770.0)); /* en Voltios */
  //I = V_i/5.0 * 1000; /* mA */
  //I_1=V_i_1/5.0*1000;
  tf = micros();
  tiempo = tf - ti;
}

/*******************************************************************
 * control_loop - Función del lazo de control de la carga
 */

void control_loop() {

  float err_act, e1_med, e1_stdev, e2_med, e2_stdev;

  err_act = I - set_point;
  err_stat = insert_calcula_stat_array(err_hist, N_DATA_ERR, err_act);
#if USE_PID == 1
  /*if (abs(err_act) < 10)
  { //we're close to setpoint, use conservative tuning parameters
    pid.SetTunings(consKp, consKi, consKd);
  }
  else
  {
    //we're far from setpoint, use aggressive tuning parameters
    pid.SetTunings(aggKp, aggKi, aggKd);
  }*/
  pid.Compute();
#else
  dac = dac - err_act / 600.0 * 255 * 0.1; /* Ajustar este parámetro para que la señal sea más estable I fuente, y no fluctue tanto*/
#endif

  if (dac < 0) dac = 0;
  if (dac > 255) dac = 255;
  dacWrite(PIN_DAC, dac);

  if (set_point >= 60) {
    e1_med=0.02;
    e1_stdev=0.02;
    e2_med=0.03;
    e2_stdev=0.03;
  } else if (set_point >=25) {
    e1_med=0.04;
    e1_stdev=0.04;
    e2_med=0.065;
    e2_stdev=0.065;
  } else if (set_point >=10) {
    e1_med=0.07;
    e1_stdev=0.07;
    e2_med=0.1;
    e2_stdev=0.1;
  } else  {
    e1_med=0.1;
    e1_stdev=0.1;
    e2_med=0.13;
    e2_stdev=0.13;
  }
  
  if (abs(err_stat.media / set_point) < e1_med && err_stat.desviacion / set_point < e1_stdev) {
    err_fix = true;
  } else {
    if (err_fix && abs(err_stat.media / set_point) < e2_med && err_stat.desviacion / set_point < e2_stdev)
      err_fix=true;
    else 
      err_fix = false;
  }
}

/*-------------------------------------------------------------------------------------------------------------------------------
 * busca_max - Función que busca el máximo de potencia en un rango
 * 
 */
 
PMAX busca_max(float mini, float maxi, float inc) {
  PMAX ret;
  float P_max, P_aux, I_aux, V_aux;
  long tini;
  P_max = 0;
  ret.I = 0;
  ret.V = 0;
  ret.P = 0;
  ret.I_last = -1;
  ret.status = 1;

  for (float k = mini; k <= maxi ; k = k + inc) {
    //Serial.print("I_test:");
    //Serial.println(k);
    set_point = k;
    err_fix=false;
    delay(50);
    tini = micros();
    while (!err_fix) {
      if ( (micros() - tini) / 1000.0 > BUSCA_MAX_TIMEOUT) {
        // No hemos conseguido fijar el setpoint en el tiempo fijado
        break;
      }
      delay(10);
    }

    if (err_fix) {    // Si el lazo de control fija el valor, tomamos los datos
      //Serial.println("Error fijado");
      //delay(100);
      I_aux = I;
      V_aux = V_celula;
      P_aux = I_aux * V_aux;
      ret.I_last = k;
      if (P_aux > P_max) {
        ret.I = I_aux;
        ret.V = V_aux;
        ret.P = P_aux;
        if (ret.status >=0) ret.status = 0;
        P_max = P_aux;
      }
    } else {    // Si ha ocurrido un timeout
      if (ret.status>=0) ret.status = -1;
      else ret.status=ret.status-1;
    }
    if (TEST_PPS) delay(1000);
  }
  set_point=0;
  return ret;
}

/*************************************************************************
 * busca_mppt - Busca el punto de máxima potencia de un panel
 */

PMAX busca_mppt(int panel)
{
  PMAX ret,ret_max;
  float imin,imax,iinc;
  digitalWrite(PIN_SEL, panel);
  delay(100);

  imin=IMIN;
  imax=IMAX;
  iinc=IINC;

  Serial.printf("\nBuscando MPPT en panel %d\n", panel);

  ret_max.P=-1;
  while (iinc>MININC) {
    Serial.printf("\nIMIN: %.2f mA, IMAX: %.2f mA, IINC: %.2f mA\n",imin, imax, iinc);
    
    ret = busca_max(imin, imax, iinc);
    Serial.print("Stat:");
    Serial.print(ret.status);
    Serial.print(",");
    Serial.print("I_last:");
    Serial.print(ret.I_last);
    Serial.print(",");
    Serial.print("Int_MAX:");
    Serial.print(ret.I);
    Serial.print(",");
    Serial.print(" Vol_max:");
    Serial.print(ret.V);
    Serial.print(",");
    Serial.print("Pot_max:");
    Serial.println(ret.P);
    imin=ret.I-iinc*1.1;
    imax=ret.I+iinc*1.1;
    iinc=iinc/2;
    if (imin < IMIN) imin=IMIN;
    if (imax > IMAX) imax=IMAX;

    if (ret.P > ret_max.P) {
      ret_max=ret;
      Serial.printf("Nuevo MPPT: %.0f mW\n",ret_max.P);
    } else {
      Serial.println("Warning: el nuevo muestreo no ha encontrado MPPT");
    } 
  } 

  Serial.printf("\nPanel: %d\nPmax: %.0f mW\n\n",panel,ret_max.P);
  
  return ret_max;
}

//--------------------------------------------------------//
//---------------RELATIVO AL SEGUIDOR SOLAR---------------//
//--------------------------------------------------------//

ANGULO calculo_angular(int Year, float Local_Time, int hora, int Day_of_Year){
  ANGULO ret;
  float x, EqTime, Declinacion, Time_offset, TimeSolar, Angulo_horario, Alt, Alt_grados, Acim, Acim_grados,Declinacion_grados;
  int dias_year;
  
  //Veamos si el año es bisiesto o no:
  if (Year % 4 == 0 && Year % 100 != 0 || Year % 400 == 0)
    dias_year = 366; //El año es bisiesto!
  else
    dias_year = 365; //No bisiesto!
    
  //Una vez sabemos si el año es bisiesto o no, podemos proceder a calcular el resto de variables. Año fraccionado en Radianes:
  x = 2*PI/dias_year * (Day_of_Year - 1 + (hora - 12)/24);
  //Con x, podemos obtener la ecuacion del tiempo (en minutos) y la declinación (en radianes):
  EqTime = 229.18 * (0.000075 + 0.001868 * cos(x) - 0.032077 * sin(x) - 0.014615 * cos(2*x)-0.040849 * sin(2*x));
  Declinacion =  0.006918 - 0.399912 * cos(x) + 0.070257 * sin(x) - 0.006758 * cos(2*x) + 0.000907 * sin(2*x) - 0.002697 * cos(3*x) + 0.00148 * sin(3*x);
  Declinacion_grados = Declinacion * 360 / (2*PI);
  //Para calcular el tiempo solar necesitamos dos expresiones. La primera asociada al desplazamiento temporal (en minutos):
  Time_offset = EqTime + 4 * Latitud;
  TimeSolar = Local_Time + Time_offset / 60; //Tiempo solar en horas
  Angulo_horario = 15 * (TimeSolar - 12);  //En grados
  Alt = asin(sin(Declinacion) * sin(Longitud*2*PI/360) + cos(Longitud*2*PI/360) * cos(Declinacion) * cos(Angulo_horario*2*PI/360)); //En radianes
  Alt_grados = Alt * 360 / (2*PI);
  Acim = asin((cos(Declinacion) * sin(Angulo_horario*2*PI / 360)) / cos(Alt)); // En radianes
  Acim_grados = Acim *360 / (2*PI); 
  ret.Altitud  = Alt_grados; 
  ret.Azimutal = Acim_grados; 
  Serial.println(ret.Altitud);
  Serial.println(ret.Azimutal);
  return ret;
}  

void movimiento(float Elevacion, float Acimutal){
  int OffSet_azimut = +9;
  int OffSet_elevacion = 0;
  if (Elevacion >= 0){
      servo.move(1, (120 + Acimutal + OffSet_azimut), 50);
      servo.move(2, (120 + Elevacion + OffSet_elevacion), 50);
  }
}

//------------------------------------------------------------------------------------------------------------------------------------
void display_msg() {
  //Serial.print("t_us:");
 // Serial.print(tiempo);
  //Serial.print(",");
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

  //Serial.println("------- Valores control -------------");

  /*Serial.print("ERR_Media:");
  Serial.print(err_stat.media);
  Serial.print(",");
  Serial.print("ERR_Deviacion:");
  Serial.print(err_stat.desviacion);
  Serial.print(",");
  Serial.print("ERR_Tiempo:");
  Serial.print(err_stat.tiempo);
  Serial.print(",");*/
  Serial.print("Setpoint:");
  Serial.print(set_point);
  Serial.print(",");
  Serial.print("I:");
  Serial.print(I);
  Serial.print(",");
  Serial.print("V*10:");
  Serial.print(V_celula*10);
  Serial.print(",");
  Serial.print("ERROR:");
  Serial.print(set_point-I);
  Serial.print(",");  
  Serial.print("ERR_FIX:");
  Serial.print(err_fix*50);
  Serial.print(",");  
  Serial.print("DAC:");
  Serial.println(dac);



  //Serial.println("------------------------");
}

void setup() {
  //MONITOR SERIE:
  Serial.begin(115200);
  err_fix = false;
  pid.SetMode(AUTOMATIC);
  pid.SetSampleTime(CTRL_TIME * 1000);

#if TESTING == false

  //INICIA CONEXIÓN WiFi:
  WiFi.begin(ssid,password); /* Nos conectamos al router WiFi*/
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi conectado!");
  ThingSpeak.begin(cliente); /* Inicializamos servidor ThingSpeak */

#endif

  Serial.println("");
  Serial.println("Configurando ADCs e I/O");
  
  analogReadResolution(11);
  analogSetAttenuation(ADC_6db);
  pinMode(PIN_SEL, OUTPUT);
  digitalWrite(PIN_SEL, LOW); /* Entrada izq=3.3=fijo, Drch=0=movil*/
  pinMode(PIN_PWM, OUTPUT);
  analogWrite(PIN_PWM, 127);

  // Arrancamos Tickers

  tick_tension.attach(SAMPLE_TIME, tension);
  tick_corriente.attach(SAMPLE_TIME, intensidad);
  tick_control.attach(CTRL_TIME, control_loop);
#if TESTING == true
  tick_disp.attach(DISP_TIME, display_msg);
#endif

//--------------------------------------------------------//
//---------------RELATIVO AL SEGUIDOR SOLAR---------------//
//--------------------------------------------------------//

  //CONFIGURACIÓN DE LA HORA:
  configTime(gmtOffset_sec,daylightOffset_sec,ntpServer);

  //Esperamos a que el tiempo se configure bien
  struct tm timeinfo;
  getLocalTime(&timeinfo,100000);
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  //RELATIVO AL MOVIMIENTO DE LOS SERVOS:
  servo.init(2);
  servo.setDebug(true);  // Do not view servo detction here
  servo.begin();
  servo.setDebug(true);
  Serial.println(F("\n\nAvailable servo:"));
  servoCnt = 0;
  for (int id = 1; id <= 2; id++) {
    if (servo.exists(id)) {
      Serial.print(F("Servo ID="));
      Serial.print(id);
      Serial.println(F(" is avalable."));
      servo.setLedOff(id); // turn the led off (default on when power on)
      servoCnt++;
    }
  }
  Serial.println();
  if (servoCnt > 0) {
    Serial.print(servoCnt);
  } else {
    Serial.print(F("No "));
  }
  Serial.println(F(" servo detected."));
}

void loop() {
  
  //--------------------------------------------------------//
  //---------------RELATIVO AL SEGUIDOR SOLAR---------------//
  //--------------------------------------------------------// 
  
  ANGULO retu;
  hora = rtc.getHour(true);
  Year = rtc.getYear();
  Day_of_Year = rtc.getDayofYear() + 1; //[0-365] Hay que sumarle 1 porque inicia en el 0.
  Local_Time = rtc.getHour(true) + rtc.getMinute()/60.0 + rtc.getSecond()/3600.0; //En horas
  retu = calculo_angular(Year,Local_Time,hora,Day_of_Year);
  Elevacion = retu.Altitud;
  Acimutal  = retu.Azimutal;
  movimiento(Elevacion,Acimutal);
  
  //--------------------------------------------------------//
  //---------------RELATIVO A LA CARGA ACTIVA---------------//
  //--------------------------------------------------------// 
  
  PMAX ret;
  long tini,tfin;
  tini=millis(); 

#if TESTING == true
 
  digitalWrite(PIN_SEL,LOW);
  delay(100);
  set_point=5;
  delay(4000);
  set_point=10;
  delay(4000);
  set_point=20;
  delay(4000);
  set_point=30;
  delay(4000);
  set_point=40;
  delay(4000);
  set_point=50;
  delay(4000);
  set_point=60;
  
#else
  
  Serial.println("");
  Serial.println("Nueva iteración de búsqueda");

  //--------------------------------------------------------//
  //---------------------RELATIVO A IOT---------------------//
  //--------------------------------------------------------// 

  // Primer panel 
  
  ret=busca_mppt(LOW);
  
  ThingSpeak.setField(1,ret.I); // Se crea el campo
  ThingSpeak.setField(2,ret.V);
  ThingSpeak.setField(3,ret.P);
  
  // El otro panel
  
  ret=busca_mppt(HIGH);
 
  ThingSpeak.setField(4,ret.I);
  ThingSpeak.setField(5,ret.V);
  ThingSpeak.setField(6,ret.P);
  
  // Variables angulares
  
  ThingSpeak.setField(7,retu.Altitud);  
  ThingSpeak.setField(8,retu.Azimutal);

  //Subimos los datos a ThingSpeak
  
  ThingSpeak.writeFields(1687542,"3QUZEAC5CQG1GPIJ"); //Subimos los datos cada 2 min
  Serial.println("Datos subidos a ThingSpeak");

 #endif

 tfin=millis();
 int dela;

 dela=15000-(tfin-tini);
 if (dela < 0) dela=0;
 delay(dela);  
}
