#ifndef IRline_h
#define IRline_h
#include "Arduino.h"

class IRline {
  public:
    IRline(uint8_t *pubpins, uint8_t pubnumIR, uint8_t pubmuxpin = 0, bool pubmode = 1);
    void calibrateIR(uint16_t waittime = 5000);
    float getPlusUpdateIR(uint16_t debouncetime = 0);
    float getError();
    void showIR();
    float PID();
  private:
    uint8_t ci[16][3] = {
      { 0, 0, 0 },
      { 0, 0, 1 },
      { 0, 1, 0 },
      { 0, 1, 1 },
      { 1, 0, 0 },
      { 1, 0, 1 },
      { 1, 1, 0 },
      { 1, 1, 1 },
    };
    uint8_t valsensors;
    uint16_t mid[8];
    float error = 0, lasterror = 0;
    bool mode;
    uint8_t numIR;
    uint8_t muxpin;
    uint8_t *pins;
    unsigned long pastmillis = 0;
};

#endif
