/* BUSCA EL VALOR DE LA TENSIÓN. RECORDEMOS QUE ESTE PROGRAMA ES PARA ENTRADA ADC Y DAC DE [0-3.3] V */
const int portPin = 34; /* Pin esp32 para leer el ADC2 (entrada analógica) */

int ADC2_analog ; /* lectura analógica de ADC2 */
float setpoint ; /* Fijamos un valor de la tensión deseada en V */
float ADC2 ; /* ADC2 en V */
int cambio ; /* [0-255] Digital */
int dac=0 ;
void setup() {
  Serial.begin(9600);
  delay(100);
}

void loop() {
  /*Inicializar el Dac fuera del loop a un valor aleatorio para que vaya después cambiando*/
  ADC2_analog = analogRead(portPin); 
  ADC2 = (float(ADC2_analog)/4095)*3.3*2; 
  setpoint = (analogRead(35)/4095.0)*3.3;
  Serial.println(setpoint);
  
  if ((abs(ADC2 - setpoint)) <= 0.01){
    Serial.println("SE HA ENCONTRADO EL VALOR DESEADO");
  } else {
     if ((ADC2 - setpoint) > 0){
       Serial.println("Decrementar DAC/Potenciómetro");
       cambio = int(dac-(((abs(ADC2 - setpoint)/2.0)/3.3)*255));
       if (cambio < 0) cambio=0;
       dac=cambio;
       dacWrite(25,dac);
     } 
     if ((ADC2 - setpoint) < 0){
      Serial.println("Incrementar el DAC/potenciómetro");
      cambio = int(dac+(((abs(ADC2 - setpoint)/2.0)/3.3)*255));
      if (cambio>255) cambio=255;
      dac=cambio;
      dacWrite(25,dac);
     }
   } 
    
  Serial.print(" SETPOINT: ");  
  Serial.print(setpoint);
  Serial.print(" ADC2: "); 
  Serial.print(ADC2);
  Serial.print(" DAC: ");
  Serial.println(dac);
  
  delay(4000);
}
