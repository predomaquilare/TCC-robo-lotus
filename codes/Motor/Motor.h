#ifndef Motor_h
#define Motor_h
#include "Arduino.h"

class Motor {
  public:
    Motor(uint8_t *pubpins);
    void frente(int v = 4095);
    void esquerda(int v = 4095);
    void direita(int v = 4095);
    void PIDctrl(float pid, byte sensors);
  private:
    uint8_t valsensores;
    void noventagrausesq();
    void noventagrausedir()
    unsigned long previousMillis = 0;
    uint8_t *pins;
};
#endif

