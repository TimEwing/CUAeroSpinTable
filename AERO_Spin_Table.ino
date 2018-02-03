
#define PIN_STP 3
#define PIN_DIR 2
#define PIN_MS1 6
#define PIN_MS2 5
#define PIN_MS3 4
#define PIN_EN  7

// NOTE: Spin table has 800 steps/rev

float start_time = 0;
float stop_time = 0;

int step_count = 0;

int mode = ' ';
int step_delay = 25;
bool dir = false;
bool MS1 = false;
bool MS2 = false;
bool MS3 = false;

void setup() 
{
  // Setup Serial for debug
  Serial.begin(9600);
  Serial.println("Starting turn");
  // Setup pinmodes
  pinMode(PIN_STP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_MS1, OUTPUT);
  pinMode(PIN_MS2, OUTPUT);
  pinMode(PIN_MS3, OUTPUT);
  pinMode(PIN_EN, OUTPUT);
  // Set stepper driver pins to default
  reset_big_easy_pins();
}

void loop() 
{
  parse_serial();
  
  if(step_count > 0)
  {
    digitalWrite(PIN_STP, HIGH);
    delay(step_delay);
    digitalWrite(PIN_STP, LOW);
    delay(step_delay);
    step_count--;
  }
  else if(stop_time == 0)
  {
    stop_time = millis();
  }
  else
  {
    Serial.println((stop_time - start_time) / 1000);
  }
}

void parse_serial()
{
  bool get_input = true;
  char m = 0;
  while(Serial.available() > 0)
  {
    char in = ' ';
    in = Serial.read();

    switch (in)
    {
      case 'g': // go
        digitalWrite(PIN_EN, LOW);
        get_input = false;
        step_count = 800;
        start_time = millis();
        stop_time = 0;
        mode = ' ';
        continue; // This will make the while loop continue, not the switch statement
      case 's': // stop
        digitalWrite(PIN_EN, HIGH);
        get_input = false;
        mode = ' ';
        continue;
      case 'm': // set MS*
        mode = 'm';
        m = 0;
        continue;
      case 't':// set step
        mode = 't';
        continue;
      case 'd':// set dir
        mode = 'd';
        continue;
    }

    switch (mode)
    {
      case 'm':
        if (m == 0) // check if we picked which pin to set
        {
          switch(in)
          {
            case '1':
              m = 1;
              break;
            case '2':
              m = 2;
              break;
            case '3':
              m = 3;
              break;
          }
        }
        else
        {
          switch(m)
          {
            case 1:
              digitalWrite(PIN_MS1, in == '0' ? LOW : HIGH);
              m = 0;
              mode = ' ';
              Serial.println(in == '0' ? "Set MS1 to LOW" : "Set MS1 to HIGH");
              break;
            case 2:
              digitalWrite(PIN_MS1, in == '0' ? LOW : HIGH);
              m = 0;
              mode = ' ';
              Serial.println(in == '0' ? "Set MS2 to LOW" : "Set MS2 to HIGH");
              break;
            case 3:
              digitalWrite(PIN_MS1, in == '0' ? LOW : HIGH);
              m = 0;
              mode = ' ';
              Serial.println(in == '0' ? "Set MS3 to LOW" : "Set MS3 to HIGH");
              break;
          }
        }
        break;
      case 't':
        step_delay = Serial.parseInt();
        Serial.println(step_delay);
        mode = ' ';
        break;
      case 'd':
        digitalWrite(PIN_DIR, dir ? LOW : HIGH);
        Serial.println(dir ? "LOW" : "HIGH");
        dir = !dir;
        mode = ' ';
        break;
    }
  }
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

