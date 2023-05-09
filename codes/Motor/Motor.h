#ifndef Motor_h
#define Motor_h
#include "Arduino.h"

class Motor {
  public:
    Motor(byte *pubpins);
    void frente(int v = 4095);
    void esquerda(int v = 4095);
    void direita(int v = 4095);
    void PIDctrl(float pid, byte sensors);
  private:
    byte valsensores;
    unsigned long previousMillis = 0;
    void noventagrausesq() {
      unsigned long currentMillis = millis();
      frente(4095 / 2);
      while (millis() - previousMillis < 1000) {}
      while ((valsensores & 0b00000110) == 0b00000110) {
        esquerda();
      }
    }
    void noventagrausdir() {
      unsigned long currentMillis = millis();
      frente(4095 / 2);
      while (millis() - previousMillis < 1000) {}
      while ((valsensores & 0b01100000) == 0b01100000) {
        direita();
      }
    }
    byte *pins;
};
#endif