#include "Motor.h"

Motor::Motor(byte *pubpins) {
  pins = pubpins;
  for (int i = 0; i < 4; i++) {

#ifdef Esp32
    pinMode(pins[i], OUTPUT);
    ledcSetup(1, 5000, 12);
    ledcSetup(2, 5000, 12);
    ledcAttachPin(pins[4], 1);
    ledcAttachPin(pins[5], 2);
#endif

#ifdef Arduino
    pinMode(pins[i], OUTPUT);
#endif
  }
}



void Motor::frente(int v) {
#ifdef Esp32
  ledcWrite(1, v);
  ledcWrite(2, v);
  digitalWrite(pins[0], 0);
  digitalWrite(pins[1], 1);
  digitalWrite(pins[2], 1);
  digitalWrite(pins[3], 0);
#endif

#ifdef Arduino
  digitalWrite(pins[0], 0);
  digitalWrite(pins[1], 1);
  digitalWrite(pins[2], 1);
  digitalWrite(pins[3], 0);
#endif
}

void Motor::direita(int v) {


#ifdef Esp32
  ledcWrite(1, v);
  ledcWrite(2, v);
  digitalWrite(pins[0], 1);
  digitalWrite(pins[1], 0);
  digitalWrite(pins[2], 1);
  digitalWrite(pins[3], 0);
#endif

#ifdef Arduino
  digitalWrite(pins[0], 1);
  digitalWrite(pins[1], 0);
  digitalWrite(pins[2], 1);
  digitalWrite(pins[3], 0);
#endif


}

void Motor::esquerda(int v) {
#ifdef Esp32
  ledcWrite(1, v);
  ledcWrite(2, v);
  digitalWrite(pins[0], 0);
  digitalWrite(pins[1], 1);
  digitalWrite(pins[2], 0);
  digitalWrite(pins[3], 1);
#endif

#ifdef Arduino
  digitalWrite(pins[0], 0);
  digitalWrite(pins[1], 1);
  digitalWrite(pins[2], 0);
  digitalWrite(pins[3], 1);
#endif
}

void Motor::PIDctrl(float pid, byte sensors) {
  float constant = 0.8;
  valsensores = sensors;
  #define direita 2
  #define esquerda 1
#ifdef Esp32
  int v = 4095 * constant;
#endif

#ifdef Arduino
  int v = 1023 * constant;
#endif


  int a = 0, b = 0;
  a = v + pid;
  b = v - pid;

#ifdef Esp32
  if (a > 4095) a = 4095;
  if (a < -4095) a = -4095;
  if (b < -4095) b = -4095;
  if (b > 4095) b = 4095;
#endif

#ifdef Arduino
  if (a > 1023) a = 1023;
  if (a < -1023) a = -1023;
  if (b < -1023) b = -1023;
  if (b > 1023) b = 1023;
#endif


  if ((sensors != 0b11111100) || (sensors != 0b00111111)) {
    if (pid > 0) { // vira para a direita
      if (b > 0) {
#ifdef Esp32
        ledcWrite(esquerda, a);
        ledcWrite(direita, b);
        digitalWrite(pins[0], 0);
        digitalWrite(pins[1], 0);
        digitalWrite(pins[2], 1);
        digitalWrite(pins[3], 0);
#endif

#ifdef Arduino
        analogWrite(pins[4], a);
        analogWrite(pins[5], b);
        digitalWrite(pins[0], 0);
        digitalWrite(pins[1], 0);
        digitalWrite(pins[2], 1);
        digitalWrite(pins[3], 0);
#endif

      } else {

#ifdef Esp32
        ledcWrite(esquerda, a);
        ledcWrite(direita, -b);
        digitalWrite(pins[0], 1);
        digitalWrite(pins[1], 0);
        digitalWrite(pins[2], 1);
        digitalWrite(pins[3], 0);
#endif

#ifdef Arduino
        analogWrite(pins[4], a);
        analogWrite(pins[5], -b);
        digitalWrite(pins[0], 1);
        digitalWrite(pins[1], 0);
        digitalWrite(pins[2], 1);
        digitalWrite(pins[3], 0);
#endif

      }
    } else if (pid < 0) { // vira para esquerda
      if (a > 0) {
#ifdef Esp32
        ledcWrite(esquerda, a);
        ledcWrite(direita, b);
        digitalWrite(pins[0], 0);
        digitalWrite(pins[1], 1);
        digitalWrite(pins[2], 0);
        digitalWrite(pins[3], 0);
#endif

#ifdef Arduino
        analogWrite(pins[4], b);
        analogWrite(pins[5], a);
        digitalWrite(pins[0], 0);
        digitalWrite(pins[1], 1);
        digitalWrite(pins[2], 0);
        digitalWrite(pins[3], 0);
#endif

      } else {
#ifdef Esp32
        ledcWrite(esquerda, -a);
        ledcWrite(direita, b);
        digitalWrite(pins[0], 0);
        digitalWrite(pins[1], 1);
        digitalWrite(pins[2], 0);
        digitalWrite(pins[3], 1);
#endif

#ifdef Arduino
        analogWrite(pins[4], b);
        analogWrite(pins[5], -a);
        digitalWrite(pins[0], 0);
        digitalWrite(pins[1], 1);
        digitalWrite(pins[2], 0);
        digitalWrite(pins[3], 0);
#endif
      }
    } else if (pid == 0) {
#ifdef Esp32
      ledcWrite(esquerda, a);
      ledcWrite(direita, b);
      digitalWrite(pins[0], 0);
      digitalWrite(pins[1], 1);
      digitalWrite(pins[2], 1);
      digitalWrite(pins[3], 0);
#endif

#ifdef Arduino

#endif
    }
  } else {
    if (sensors == 0b11111100) {
      noventagrausesq();
    }
    else if (sensors == 0b00111111) {
      noventagrausdir();
    }
  }
  Serial.print(a);
  Serial.print(" ");
  Serial.print(b);
}