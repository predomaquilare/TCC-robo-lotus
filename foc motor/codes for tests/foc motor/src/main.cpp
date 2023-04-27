#include <Arduino.h>

const int potentiometer = A3;     // The speed/angle potentiometer is connected on this pin

const int Motor_phase_A = 25;      //Pin for driver input of phase A
const int Motor_phase_B = 26;     //Pin for driver input of phase B
const int Motor_phase_C = 27;     //Pin for driver input of phase C
int phases[3] = {25,26,27};
//Variables used in the code
int16_t SINE_A = 0;               //Initial angle value of the phase A
int16_t SINE_B = 120;             //Initial angle value of the phase B
int16_t SINE_C = 240;             //Initial angle value of the phase C
int poles = 11;                   /*Amount of poles of the motor (change this value if the motor is not
                                  getting to a full rotation. For example, my motor has 28 poles but I had
                                  to add "11" in order to make a full rotation*/
uint32_t adc_read = 0;            //Variable to store the pot ADC value.

void moving();                             

void setup() {
  Serial.begin(115200);
  for(int i = 0; i < 3; i++) {
    ledcSetup(i+1, 5000, 8);
    ledcAttachPin(phases[i], i+1);
  }
}
 
void loop() {
  moving(); // The motor is moving. 
}



void moving()
{   
  SINE_A = SINE_A + 1;      //Add 1 so the rotation will continue 1 by 1
  SINE_B = SINE_A + 120;    //We have a 120 phase difference betweeen phase A and B
  SINE_C = SINE_B + 120;    //We have a 120 phase difference betweeen phase B and C
   
  //Range calculation of Sine Signal
  SINE_A = SINE_A%360;    //Keep the values between 0 and 359
  SINE_B = SINE_B%360;    //Keep the values between 0 and 359
  SINE_C = SINE_C%360;    //Keep the values between 0 and 359
 
  //Calculate the PWM values for creating a sine signal (SPWM)
  int SINE_A_PWM = sin((double)SINE_A*PI/180)*127.5+127.5;  //Multiply by PI and divide by 180 in order to pass from degrees to radians
  int SINE_B_PWM = sin((double)SINE_B*PI/180)*127.5+127.5;  //Multiply by 127.5 and add 127.5 in order to keep the range between 0-255
  int SINE_C_PWM = sin((double)SINE_C*PI/180)*127.5+127.5;  //Sine values between -1 and 1 are placed between 0-255 for PWM. 


  ledcWrite(phases[0], SINE_A_PWM);             //You might change the 0.7 value for more torque...
  ledcWrite(phases[1], SINE_A_PWM);               //You might change the 0.7 value for more torque...
  ledcWrite(phases[2], SINE_A_PWM);              //You might change the 0.7 value for more torque...
  
  adc_read = 600;
  delay(adc_read/15);         //Controlling the loop delay, we control the speed of rotation 
}
