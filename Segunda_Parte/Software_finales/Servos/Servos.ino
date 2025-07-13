#include "ESP32Time.h" //ESP32Time.h es una biblioteca de tiempos que hace una sincronización con el servidor NTP.
#include "WiFi.h" //WiFi.h es la libreria wifi para conectar el microcontrolador a la red wifi.
#include "UBTech.h" //UBTech.h es una librería que se encarga del control de los servomotores.

#define PI 3.14159265358979324 //Número PI
#define Longitud 38.886 //En grados 
#define Latitud -7.007 //En grados. Postitivo al Este del Meridiano_G y negativo al Oeste

//Nombre y contraseña del WiFi:
const char* ssid = "APTest1"; //Nombre red WiFi
const char* password = "isoTech25"; //Contraseña red WiFi

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

ANGULO calculo_angular(int Year, float Local_Time, int hora, int Day_of_Year){
  ANGULO ret;
  float x, EqTime, Declinacion, Time_offset, TimeSolar, Angulo_horario, Alt, Alt_grados, Acim, Acim_grados,Declinacion_grados;
  int dias_year;
  //const int OFFSET_ALTITUD = ;
  //const int OFFSET_AZIMUTAL = ;
  
  //Veamos si el año es bisiesto o no:
  if (Year % 4 == 0 && Year % 100 != 0 || Year % 400 == 0)
    dias_year = 366; //El año es bisiesto!
  else
    dias_year = 365; //No bisiesto!
    
  //Una vez sabemos si el año es bisiesto o no, podemos proceder a calcular el resto de variables. Año fraccionado en Radianes:
  x = 2*PI/dias_year * (Day_of_Year - 1 + (hora - 12)/24);
  Serial.print("x : ");
  Serial.println(x);
  //Con x, podemos obtener la ecuacion del tiempo (en minutos) y la declinación (en radianes):
  EqTime = 229.18 * (0.000075 + 0.001868 * cos(x) - 0.032077 * sin(x) - 0.014615 * cos(2*x)-0.040849 * sin(2*x));
  Serial.print("EqTimne : ");
  Serial.println(EqTime);
  Declinacion =  0.006918 - 0.399912 * cos(x) + 0.070257 * sin(x) - 0.006758 * cos(2*x) + 0.000907 * sin(2*x) - 0.002697 * cos(3*x) + 0.00148 * sin(3*x);
  Declinacion_grados = Declinacion * 360 / (2*PI);
  Serial.print("Declinacion : ");
  Serial.println(Declinacion_grados);
  //Para calcular el tiempo solar necesitamos dos expresiones. La primera asociada al desplazamiento temporal (en minutos):
  Time_offset = EqTime + 4 * Latitud;
  Serial.print("Time_offset : ");
  Serial.println(Time_offset);
  TimeSolar = Local_Time + Time_offset / 60; //Tiempo solar en horas
  Serial.print("Tiempo solar en horas: ");
  Serial.println(TimeSolar,4);
  //-----------------------------------------------------------------------
  Angulo_horario = 15 * (TimeSolar - 12);  //En grados
  Serial.print("Angulo Horario: ");
  Serial.println(Angulo_horario);
  Alt = asin(sin(Declinacion) * sin(Longitud*2*PI/360) + cos(Longitud*2*PI/360) * cos(Declinacion) * cos(Angulo_horario*2*PI/360)); //En radianes
  Alt_grados = Alt * 360 / (2*PI);
  Acim = asin((cos(Declinacion) * sin(Angulo_horario*2*PI / 360)) / cos(Alt)); // En radianes
  Acim_grados = Acim *360 / (2*PI); 
  ret.Altitud  = Alt_grados; 
  ret.Azimutal = Acim_grados; 
  return ret;
}  
//¿Cómo paso la estructura?
void movimiento(float Elevacion, float Acimutal){
  int OffSet_azimut = 0;
  int OffSet_elevacion = 0;
  if (Elevacion >= 0){
      servo.move(1, (120 + Acimutal + OffSet_azimut), 50);
      servo.move(2, (120 + Elevacion + OffSet_elevacion), 50);
  }
}

void setup() {
  //MONITOR SERIE:
  Serial.begin(115200);
  
  //INICIA CONEXIÓN WiFi:
  WiFi.begin(ssid,password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println("");
  Serial.println("WiFi Conectado");
  
  //CONFIGURACIÓN DE LA HORA:
  configTime(gmtOffset_sec,daylightOffset_sec,ntpServer);

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
  ANGULO ret;
  Serial.println("---------------------------------------------------------------");
  Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));  //Retorna DiaSemana, Mes, día, Año, h:mm:s
  hora = rtc.getHour(true);
  Year = rtc.getYear();
  Day_of_Year = rtc.getDayofYear() + 1; //[0-365] Hay que sumarle 1 porque inicia en el 0.
  Local_Time = rtc.getHour(true) + rtc.getMinute()/60.0 + rtc.getSecond()/3600.0; //En horas
  Serial.print("Local Time UTC: ");
  Serial.println(Local_Time,4);
  Serial.print("hora: ");
  Serial.println(hora);
  Serial.print("Año: ");
  Serial.println(Year);
  Serial.print("Dia del año (1-366): ");
  Serial.println(Day_of_Year);
  ret = calculo_angular(Year,Local_Time,hora,Day_of_Year);
  Serial.print("Angulo Altitud: ");
  Serial.println(ret.Altitud);
  Serial.print("Angulo Azimutal: ");
  Serial.println(ret.Azimutal);
  Elevacion = ret.Altitud;
  Acimutal  = ret.Azimutal;
  movimiento(Elevacion,Acimutal);
  delay(5000);
}
