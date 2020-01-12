#define ledPin9 9
// Pinii motor 1
#define mpin00 5
#define mpin01 6
// Pinii motor 2
#define mpin10 3
#define mpin11 11
int state = 0;
 
void setup() {

  digitalWrite(ledPin9, LOW);
  digitalWrite(mpin00, 0);
  digitalWrite(mpin01, 0);
  digitalWrite(mpin10, 0);
  digitalWrite(mpin11, 0);
  
  pinMode (mpin00, OUTPUT);
  pinMode (mpin01, OUTPUT);
  pinMode (mpin10, OUTPUT);
  pinMode (mpin11, OUTPUT);
 
  Serial.begin(38400); // Default communication rate of the Bluetooth module
}
 
void StartMotor (int m1, int m2, int forward, int speed)
{
  if (speed == 0) // oprire
  {
    digitalWrite(m1, 0);
    digitalWrite(m2, 0);
  }
  else
  {
    if (forward)
    {
      digitalWrite(m2, 0);
      analogWrite(m1, speed); // folosire PWM
    }
    else
    {
      digitalWrite(m1, 0);
      analogWrite(m2, speed);
    }
  }
}

void delayStopped(int ms)
{
  StartMotor (mpin00, mpin01, 0, 0);
  StartMotor (mpin10, mpin11, 0, 0);
  delay(ms);
}
 
void loop() {
  if (Serial.available() > 0) { 
  // Verifica daca vine mesaj de pe portul serial - de la dispozitivul master
    state = Serial.read(); // Citeste din serial port
  }
  // Controlling the LED
  if (state == '!') {
    StartMotor (mpin00, mpin01, 0, 128);
    StartMotor (mpin10, mpin11, 0, 128);
    delay (500); // Cât timp e motorul pornit
    delayStopped(500);
  }
  else if (state == '@') { // fata
    StartMotor (mpin00, mpin01, 1, 128);
    StartMotor (mpin10, mpin11, 1, 128);
    delay (500);
    delayStopped(500);
  }
  else if (state == '#') {
    StartMotor (mpin00, mpin01, 0, 128);
    StartMotor (mpin10, mpin11, 1, 128);
    delay (500);
    delayStopped(500); 
  }
  else if (state == '$') {
    StartMotor (mpin00, mpin01, 1, 128);
    StartMotor (mpin10, mpin11, 0, 128);
    delay (500);
    delayStopped(500);
  }
}
