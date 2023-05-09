#include "IRline.h"

IRline::IRline(uint8_t *pubpins, uint8_t pubnumIR, uint8_t pubmuxpin, bool pubmode) {
  muxpin = pubmuxpin;
  numIR = pubnumIR;
  mode = pubmode;
  pins = pubpins;

  if (mode == 1) {
    for (uint8_t i = 0; i < numIR; i++) {
      pinMode(pins[i], INPUT);  // modo sem multiplexador
    }
  } else {
    for (uint8_t i = 0; i < 3; i++) {
      pinMode(pins[i], OUTPUT);
      pinMode(muxpin, INPUT);  // modo com multiplexador
    }
  }
}
void IRline::calibrateIR(uint16_t waittime) {
    
#ifdef Esp32
  uint16_t mins[numIR] = {4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095};
#endif

#ifdef Arduino
  uint16_t mins[numIR] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
#endif

  uint16_t calibratedvals[numIR];
  uint16_t maxs[numIR] = {0, 0, 0, 0, 0, 0, 0, 0};


  while (millis() < waittime) {
    for (uint8_t i = 0; i < numIR; i++) {
      calibratedvals[i] = analogRead(pins[i]);
      if (calibratedvals[i] > maxs[i]) maxs[i] = calibratedvals[i];
      if (calibratedvals[i] < mins[i]) mins[i] = calibratedvals[i];
      calibratedvals[i] = constrain(analogRead(pins[i]), mins[i], maxs[i]);
      mid[i] = int((mins[i] + maxs[i]) / 2.0);
    }
  }
}
float IRline::getPlusUpdateIR(uint16_t debouncetime) {
  valsensors = 0;
  if (mode == 1) {  // modo sem multiplexador
    if (millis() - pastmillis >= debouncetime) {
      pastmillis = millis();
      for (int i = 0; i < numIR; i++) {
#ifdef TestIR
        valsensors = valsensors | (((analogRead(pins[i]) > (mid[i] * 1.1)) ? 0 : 1) << i);
#endif
#ifndef TestIR
        valsensors = valsensors | (((analogRead(pins[i]) > (mid[i] * 1.1)) ? 1 : 0) << i);
#endif
      }
    }
  }
  else {
    //if (millis() - pastmillis >= debouncetime) {
    //pastmillis = millis();
    //for (int x = 0; x < numIR; x++) {
    //for (int y = 0; y < 3; y++) digitalWrite(pins[y], ci[x][y]);
    //val[x] = constrain(analogRead(muxpin), Min, Max);
    //val[x] = map(analogRead(muxpin), Min, Max, 0, 1023);
    //}
    //}
  }
  if (numIR == 8) {
    switch (valsensors) {
      case 0b10000000:
        error = 8;
        break;

      case 0b11000000:
        error = 4;
        break;

      case 0b01000000:
        error = 4;
        break;

      case 0b01100000:
        error = 3;
        break;

      case 0b00100000:
        error = 2;
        break;

      case 0b00110000:
        error = 1;
        break;

      case 0b00010000:
        error = 0;
        break;

      case 0b00011000:
        error = 0;
        break;

      case 0b00001000:
        error = 0;
        break;

      case 0b00001100:
        error = -1;
        break;

      case 0b00000100:
        error = -2;
        break;

      case 0b00000110:
        error = -2;
        break;

      case 0b00000010:
        error = -3;
        break;

      case 0b00000011:
        error = -4;
        break;

      case 0b00000001:
        error = -8;
        break;

      default:
        error = lasterror;
        break;

    }
  }
  else if (numIR == 5) {
    switch (valsensors) {
      case 0b10000:
        error = 2;
        break;

      case 0b01000:
        error = 1;
        break;

      case 0b00100:
        error = 0;
        break;

      case 0b00010:
        error = -1;
        break;

      case 0b00001:
        error = -2;
        break;
      default:
        error = lasterror;
        break;
    }
  }
  return valsensors;
}
float IRline::PID() {
  float P, I, D;
  uint16_t val = 0;
  for (uint8_t i = 0; i < 100; i++) {
    val = analogRead(35) + val;
  }
  uint16_t Kp = (val / 100);
  uint16_t Ki = 0;
  uint16_t Kd = 0;

  P = error;
  I = I + error;
  D = error - lasterror;

  #ifdef Esp32
  if (error == 0) I = 0;
  if (I > 4095) I = 4095;
  else if (I < -4095) I = -4095;
  #endif

  #ifdef Arduino
  if (error == 0) I = 0;
  if (I > 255) I = 255;
  else if (I < -255) I = -255;
  #endif

  int PID = int((Kp * P) + (Ki * I) + (Kd * D));
  lasterror = error;
  return int(PID);
}
float IRline::getError() {
  return error;
}

uint8_t IRline::showIR() {
  return valsensors;
}