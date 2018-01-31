
#define PIN_STP 3
#define PIN_DIR 2
#define PIN_MS1 6
#define PIN_MS2 5
#define PIN_MS3 4
#define PIN_EN  7

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
  // Enable
  digitalWrite(PIN_EN, LOW);
}

int k1 = 25;
int k2 = 100;

bool cont = true;
void loop() 
{
  if (!cont)
  {
    return;
  }
  if (Serial.available() > 0)
  {
    cont = false;
    Serial.println("done");
    return;
  }
  
  // Bounce back and forth
  Serial.println("Forward");
  digitalWrite(PIN_DIR, LOW);
  for(int x=0; x<k2; x++) 
  {
    digitalWrite(PIN_STP, HIGH);
    delay(k1);
    digitalWrite(PIN_STP, LOW);
    delay(k1);
  }

  Serial.println("Backward");
  digitalWrite(PIN_DIR, HIGH);
  for(int x=0; x<k2; x++)
  {
    digitalWrite(PIN_STP, HIGH);
    delay(k1);
    digitalWrite(PIN_STP, LOW);
    delay(k1);
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

