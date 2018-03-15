// Code for aerospace spin table for gyro testing.

#define PIN_STP 3
#define PIN_DIR 2
#define PIN_MS1 6
#define PIN_MS2 5
#define PIN_MS3 4
#define PIN_EN  7

#define PIN_ACT 16

int remaining_steps = 0;
int step_delay = 25; // milliseconds

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_STP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_MS1, OUTPUT);
  pinMode(PIN_MS2, OUTPUT);
  pinMode(PIN_MS3, OUTPUT);
  pinMode(PIN_EN, OUTPUT);
  
  pinMode(PIN_ACT, INPUT_PULLUP);

  reset_big_easy_pins();
  // Enable
}

void loop() {
  if(remaining_steps > 0)
  {
    digitalWrite(PIN_STP, HIGH);
    delay(step_delay);
    digitalWrite(PIN_STP, LOW);
    delay(step_delay);

    remaining_steps--;
  }
  else
  {
    if(digitalRead(PIN_ACT) == LOW)
    {
      remaining_steps = 1600;
      enable();
    }
    else
    {
      disable();
    }
  }
}

void enable()
{
  digitalWrite(PIN_EN, LOW);
}

void disable()
{
  digitalWrite(PIN_EN, LOW);
}

void reset_big_easy_pins() 
{
  digitalWrite(PIN_STP, LOW);
  digitalWrite(PIN_DIR, LOW);
  digitalWrite(PIN_MS1, LOW);
  digitalWrite(PIN_MS2, LOW);
  digitalWrite(PIN_MS3, LOW);
  digitalWrite(PIN_EN, HIGH);
}
