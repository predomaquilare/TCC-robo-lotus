#ifndef Motor_h
#define Motor_h
#include "Arduino.h"

class Motor {
  public:
    Motor(uint8_t *pubpins);
    void frente(uint16_t v = 4095);
    void esquerda(uint16_t v = 4095);
    void direita(uint16_t v = 4095);
    void PIDctrl(float pid, uint8_t sensors);
  private:
    uint8_t valsensores;
    void noventagrausesq();
    void noventagrausdir();
    unsigned long previousMillis = 0;
    uint8_t *pins;
};
#endif

