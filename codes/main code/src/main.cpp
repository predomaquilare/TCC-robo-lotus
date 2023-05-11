#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "IRline.h"
#include "Motor.h"

#define OLED
#define Esp32
//#define Arduino
//#define TestIR

void pidoled();
void oledstart();

byte pinos[8] = { 13, 12, 14, 27, 26, 25, 33, 32 };
byte m[6] = {4, 5, 18, 19, 2, 23};

Adafruit_SSD1306 display(128, 64, &Wire, -1);
IRline ir(pinos, 8);
Motor motor(m);



void setup() {
  Serial.begin(115200);
  oledstart();
  ir.calibrateIR();
}




void loop() {
  ir.getPlusUpdateIR();
  motor.PIDctrl(ir.PID(), ir.getPlusUpdateIR());
  Serial.print("    ");
  Serial.print(ir.PID());
  Serial.print("    ");
  Serial.println(ir.getPlusUpdateIR(), BIN);
  //ir.showIR();
  pidoled();
}




void oledstart() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("falha na alocação do endereço i2c do display"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
}
void pidoled() {
#ifdef OLED
  int val = 0;
  for (int i = 0; i < 100; i++) {
    val = analogRead(35) + val;
  }
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(val / 100);
  display.display();
#endif
}















