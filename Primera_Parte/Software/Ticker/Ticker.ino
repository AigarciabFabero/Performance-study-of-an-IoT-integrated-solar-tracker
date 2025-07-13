
#include <Ticker.h> /* Incluimos la librería Ticker.h */

#define N_DATA 11 // Impar
Ticker tension; /* Creamos el Objeto */
float M_e; /* Mediana */
float V_ref[N_DATA];
float Aux[N_DATA];
int i;
int j;
float paso;
float V_i; /* Tensión teórica en la fuente. Se utiliza para ver si el calibrado es bueno. */
float suma_1=0;
float media;
float V_i_1;

void mediana(){
  long ti,tf;
  ti = micros();
  /* Realizamos la toma de datos con desplazamientos almacenando los N_DATA últimos elementos. Primero desplazamos los datos */
  for (i=1; i< N_DATA; i++) {
    j=i-1;
    V_ref[j]=V_ref[i];
  }
  V_ref[N_DATA-1] = analogReadMilliVolts(34); /* Almacenamos en la última posicion el nuevo valor. */
  /* Se almacenan los datos en un vector auxiliar que posteriormente se ordenará */
  for (i=0; i<(N_DATA); i++){
    Aux[i]=V_ref[i];
  }
  for (i = 0; i<(N_DATA-1); i++){
    for (j = i+1; j < N_DATA; j++){
      if (Aux[j] < Aux[i]){
        paso = Aux[j];
        Aux[j] = Aux[i];
        Aux[i] = paso;
      }
    }
  } 
  M_e = Aux[N_DATA/2];
  float suma=0;
  for (i = 0; i<(N_DATA); i++) {
    suma+=Aux[i];
  }
  media=suma/N_DATA;
  suma_1 = 0; 
  for (i = 0; i<(N_DATA); i++) {
    suma_1+= (Aux[i]-media)*(Aux[i]-media);
  }
  tf = micros();
  V_i=1000*((M_e/1000-12080*9880*3.03/(46770.0*(12080.0+9880.0)+(9880.0*12080.0)))*(46770.0*(12080.0+9880.0)+(9880.0*12080.0))/(12080.0*46770.0));
  V_i_1=1000*((media/1000-12080*9880*3.03/(46770.0*(12080.0+9880.0)+(9880.0*12080.0)))*(46770.0*(12080.0+9880.0)+(9880.0*12080.0))/(12080.0*46770.0));
  /*
  Serial.print(" Tiempo: "); 
  Serial.print(tf-ti);  //Tiempo que tarda en realizar todo este procedimiento 
  for (i=0; i<N_DATA; i++ ){
    Serial.print(" V_ref: ");
    Serial.print(V_ref[i]);
  }
  Serial.println("");
  for (i=0; i<N_DATA; i++ ){
    Serial.print(" AUX: ");
    Serial.print(Aux[i]);
  }
  Serial.println("");
  Serial.print(" M_e: ");
  Serial.print(M_e);
  Serial.print(" Media: ");
  Serial.print(media);
  Serial.print(" Desviación: ");
  Serial.print(sqrt(suma_1/N_DATA));
  Serial.print(" V_fuente: ");
  Serial.println(V_i);
  */
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(11);
  analogSetAttenuation(ADC_6db);
  tension.attach(0.05,mediana); /* Inicia la ejecución del Ticker */
}
void loop(){
  Serial.print(" Mediana de la tensión: ");
  Serial.print(M_e);
  Serial.print(" Media: ");
  Serial.print(media);
  Serial.print(" Desviación: ");
  Serial.print(sqrt(suma_1/N_DATA));
  /*
  Serial.print(" V_fuente mediana: ");
  Serial.print(V_i);
  Serial.print(" V_fuente media: ");
  Serial.println(V_i_1);
  */
  delay(5000);
}
