/* ESTE PROGRAMA SE EMPLEA PARA ESTUDIAR LAS FUNCIONES INTRINSECAS DEL MICRO.*/
/* #include <driver/adc.h> */

const int portPin = 34;
int ADC_D ;
float ADC;
float V_ref;

void setup() {
  Serial.begin(115200);
  analogReadResolution(11); /*Establece los bits de muestra y la resolución de lectura. 11 Bits : [0-2047]*/
  //analogSetPinAttenuation(34,ADC_6db); //Esto es seleccionando un pin concreto.
  analogSetAttenuation(ADC_6db); //nos interesa que todos los pines los interprete con la misma ganancia.
  /* 
  analogSetPinAttenuation(VP,ADC_11db); Establece la atenuación de entrada, el valor predeterminado es ADC_11db, el rango es ADC_0db, ADC_2_5db, ADC_6db ADC_11db
  ADC_0db no proporciona atenuación, por lo que IN / OUT = 1 / 1 una entrada de 3 voltios permanece a 3 voltios antes de la medición ADC
  ADC_2_5db proporciona una atenuación de modo que IN/OUT = 1 / 1.34 una entrada de 3 voltios se reduce a 2.238 voltios antes de la medición ADC
  ADC_6db proporciona una atenuación de modo que IN/OUT = 1/2 una entrada de 3 voltios se reduce a 1.500 voltios antes de la medición ADC
  ADC_11db proporciona una atenuación de modo que IN/OUT = 1 / 3.6 una entrada de 3 voltios se reduce a 0.833 voltios antes de la medición ADC
  */
  /*
  analogSetWidth(12); Establece los bits de muestra y la resolución de lectura, el valor predeterminado es de 12 bits (0 - 4095), el rango es de 9 a 12 bits
  9 bits da un rango ADC de 0-511
  10 bits da un rango ADC de 0-1023
  11 bits da un rango ADC de 0-2047
  12 bits proporciona un rango ADC de 0-4095
  */
  delay(100);
}

void loop() {
  long ti,tf;
  //ADC_D = analogRead(34);
  //ADC = (ADC_D/2047.0)*2.2;
  ti=micros();
  V_ref = analogReadMilliVolts(34); 
  tf=micros();
  //Serial.print("Tensión de salida: ");
  //Serial.print(ADC);
  Serial.print("Tiempo: ");
  Serial.print(tf-ti);
  Serial.print(" us - V_ADC: ");
  Serial.println(V_ref); /* [0-1.75] [0.142-3.175] */
  delay(2000);
}
