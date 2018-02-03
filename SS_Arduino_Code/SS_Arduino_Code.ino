// This program contains code for Arduino Due to drive motor, LED, encoder
#include "pwm01.h"

// variable declerations

 int point, angle, runPoint, incomingByte, highMotorInt, lowMotorInt, angleLCD, rateLCD, angledecLCD, ratedecLCD, dutyCycle;  // variable for incoming serial data
 int sweep = 0;
 int pointAngle = 400;
 int zeroEncoder = 0;
 int sendData = 0;
 int stopOp = 0;
 int PWMSignal = 7;
 int DIR = 5;
 int encoderPin = A0;
 float encoderValue, encoderAngle, angleReading, RPM, diff, voltage, highMotor, lowMotor;
 char anglestring[10], ratestring[10], angledecstring[12], ratedecstring[12], data[10]; // create string array

 
 void setup()
{
  
  pinMode(DIR, OUTPUT);
  
  analogReadResolution(12);
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  Serial3.begin(9600);
  delay(500);
  
  
}

void loop()
{
 // Wait for GUI inputs
 if (Serial.available() > 0)                   
 {
   incomingByte = Serial.parseInt();
   //digitalWrite(3,HIGH);
   delay(500);
   while (incomingByte == 1)
   {
     if (Serial.available() > 0)                   
     {
       RPM = Serial.parseInt();
       dutyCycle = ((RPM/100)/0.5)*65200;
       
       incomingByte = 0;
       sweep = 1;
       stopOp = 0;
       pointAngle = 370.7;
     }
   }
   while (incomingByte == 2)
   {
     if (Serial.available() > 0)                   
     {
       angle = Serial.parseInt();
       
       incomingByte = 0;
       stopOp = 0;
       point = 1;
       pointAngle = angle;
     }
   }
   if (incomingByte == 3)
   {
     stopOp = 1;
   }
   if (incomingByte == 4)
   {
     zeroEncoder = 1;
   }
 }


 // Do sweep or point depending on what GUI inputed
 if (sweep == 1)
 {
  sweepMode(dutyCycle);
  sendData = 1;
  sweep = 0;
 }
 if (point == 1)
 {
  pointModeSetup();
  point = 0;
  sendData = 1;
  runPoint = 1;
 }
 if (runPoint == 1)
 {
   pointMode(angleReading, pointAngle);
 }
 if (sendData == 1)
 {
   Serial.println(angleReading);
 }
 
 if (zeroEncoder == 1)
 {
   zeroMode();
 }
 
 if (stopOp == 1)
 {
   pwm_stop(7);
   runPoint = 0;
   sendData = 0;
   Serial.println(400);
   stopOp = 0;
 }
  
  // Run at all times for manual operation
  angleReading = encoderReading(pointAngle, zeroEncoder);
  printLCD(angleReading);
}

// Read encoder reading
float encoderReading(int pointAngle, int zeroEncoder)
{ 
  encoderValue = analogRead(encoderPin);
  encoderAngle = encoderValue * 360/4081;
  
  if (zeroEncoder == 1)
  {
    if (encoderAngle < 5)
    {
      stopOp = 1;
      zeroEncoder = 0;
    }
  }
  else
  {
    if (encoderAngle >= pointAngle) 
    {
      stopOp = 1;
    }
  }
  return encoderAngle;
} 

// Print value read from encoder to LCD
void printLCD(float angleValue) 
{ 

  angleLCD = floor(angleValue);
  angledecLCD = round((angleValue-angleLCD)*10);

  sprintf(anglestring,"%4d",angleLCD); // create strings from the numbers
  sprintf(angledecstring,"%1d",angledecLCD); 

  Serial3.write(254); // cursor to beginning of first line
  Serial3.write(128);

  Serial3.write("ANGLE:    .  \337  "); // clear display + legends
  
  Serial3.write(254); // cursor to 7th position on first line
  Serial3.write(134);
 
  Serial3.write(anglestring); // write out the angle value
  
  Serial3.write(254); // cursor to 7th position on first line
  Serial3.write(139);
 
  Serial3.write(angledecstring); // write out the angle decimal value

  delay(600); // short delay 
}

// If sweep mode, run corresponding PWM signal to motor driver
void sweepMode(uint32_t dutyCycle)
{
  
  uint32_t  pwm_duty = dutyCycle;
  uint32_t  pwm_freq = 10000;

  // Set PWM Resolution
  pwm_set_resolution(16);  

  // Setup PWM
  pwm_setup( 7, pwm_freq, 1);  // Pin 7 freq set to "pwm_freq2" on clock B
   
  // Write PWM Duty Cycle Anytime After PWM Setup
  pwm_write_duty( 7, pwm_duty );  // 50% duty cycle on Pin 7
}

// If point mode, run corresponding PWM signal
void pointModeSetup()
{
  uint32_t  pwm_freq = 10000;

  // Set PWM Resolution
  pwm_set_resolution(16);  

  // Setup PWM
  pwm_setup( 7, pwm_freq, 1);  // Pin 7 freq set to "pwm_freq2" on clock B
  
}

// If point mode, run corresponding PWM signal
void pointMode(float angleValue, int pointAngle)
{
  uint32_t  pwm_freq = 10000;
  diff = pointAngle - angleValue;
  if (diff < 0)
  {
    stopOp = 1;
  }
  else
  {
    dutyCycle = floor(1000*diff);
    if (dutyCycle > 65200)
    {
      dutyCycle = 65200;
    }
    if (dutyCycle < 17000)
    {
      dutyCycle = 17000;
    }
    uint32_t pwm_duty = dutyCycle;
    // Write PWM Duty Cycle Anytime After PWM Setup
    pwm_write_duty( 7, pwm_duty );  // 50% duty cycle on Pin 7
  }
  
    
}

// Stop motor
void stopMode()
{
  digitalWrite(PWMSignal, LOW);
}

// Zero turntable
void zeroMode()
{
  
  uint32_t  pwm_duty = 20000;
  uint32_t  pwm_freq = 10000;

  // Set PWM Resolution
  pwm_set_resolution(16);  

  // Setup PWM
  pwm_setup( 7, pwm_freq, 1);  // Pin 7 freq set to "pwm_freq2" on clock B
   
  // Write PWM Duty Cycle Anytime After PWM Setup
  pwm_write_duty( 7, pwm_duty );  // 50% duty cycle on Pin 7
}      
