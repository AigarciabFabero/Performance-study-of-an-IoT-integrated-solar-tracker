#include "UBTech.h"
SoftwareSerial ss14(14, 14, false);

// UBTech servo(&ss14);  // Without debug
UBTech servo(&ss14, &Serial);  // Debug on Serial

int servoCnt = 0;
int posicion = 110;
int posicion1 = 115;

void setup() {
  delay(100);
  Serial.begin(115200);
  Serial.println(F("\n\n\nUBTech Servo Tester\n"));
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
      //servo.setLedOff(id); // turn the led off (default on when power on)
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


  for(int i=240;i>109; i--){
     //servo.move(1, i, 100);
     int posicion = i;
     //Serial.println(i);
     //delay(500);
  }
  
  servo.move(1, 240, 200);
  delay(5000);
  servo.move(1, 0, 200);
  delay(5000);
  servo.move(2, 190, 200);
  delay(5000);
  servo.move(1, 110, 200);
  delay(5000);
  servo.move(2, 115, 200);
  delay(5000);
  //servo.move(1, 0, 200);
  //delay(5000);

}

byte ledMode = 0;
byte action = 0;

void loop() {
  //fx_checkServo();
  fx_action();
  delay(5000);
}

// Right hand only
void fx_action() {
  //if (!(servo.exists(1) && servo.exists(2) && servo.exists(3))) return;
  for (action = 0; action < 4; action++) {
    switch (action) {
      case 0:

          //servo.move(1, 240, 200);
          //delay(5000);
          //servo.move(2, 240, 200);
          //delay(20000);
          //servo.move(1, 120, 100);
          //delay(5000);
          //servo.move(2, 120, 200);
          //delay(20000);
          //servo.move(1, 0, 100);
          //delay(5000);
          //servo.move(2, 0, 200);
          //delay(15000);
        
        while (!Serial.available()) {
          
          delay(10); //espera a tener un dato
        }
          char str1 = Serial.read();
          Serial.println(str1);
              
              if (str1 == 'u'){
                posicion = posicion +1;
                servo.move(1, posicion, 100);
                Serial.println(posicion);
              }
              if (str1 == 'a'){
                posicion1 = posicion1 + 1;
                servo.move(2, posicion1, 100);
                Serial.println(posicion1);
              }
             
      break;
      /*
      case 1:
        servo.move(1, 245, 50);
        servo.move(2, 0x00, 50);
        
        delay(2000);
      break;
      /*
      case 2:
        servo.move(1, 0x0C, 100);
        servo.move(2, 0x00, 100);
        
        delay(3000);
      break;
      case 3:
        servo.move(1, 0x5A, 100);
        servo.move(2, 0x11, 100);
        
        delay(3000);
      break;
    */
    }
  }
}

void fx_checkServo() {
  if (!servoCnt) return;
  ledMode = 1 - ledMode;
  Serial.print("\nServo version / Position & LED = ");
  Serial.println(ledMode);
  byte *retBuffer;
  retBuffer = servo.retBuffer();
  for (int id = 1; id <= 16; id++) {
    if (servo.exists(id)) {
      Serial.print("Servo ");
      Serial.print(id);
      servo.getVersion(id);
      for (int i = 4; i <8; i++) {
        Serial.print(retBuffer[i] < 0x10 ? " 0" : " ");
        Serial.print(retBuffer[i], HEX);
      }
      Serial.print("  at ");
      servo.getPos(id);
      for (int i = 4; i <8; i++) {
        Serial.print(retBuffer[i] < 0x10 ? " 0" : " ");
        Serial.print(retBuffer[i], HEX);
      }
      servo.setLED(id, ledMode);
      Serial.println();
    }

  }
}
