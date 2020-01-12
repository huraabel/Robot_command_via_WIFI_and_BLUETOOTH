#define DEBUG true
#define mpin00 5
#define mpin01 6
// Pinii motor 2
#define mpin10 3
#define mpin11 11
void setup() {

  digitalWrite(mpin00, 0);
 digitalWrite(mpin01, 0);
 digitalWrite(mpin10, 0);
 digitalWrite(mpin11, 0);
 pinMode (mpin00, OUTPUT);
 pinMode (mpin01, OUTPUT);
 pinMode (mpin10, OUTPUT);
 pinMode (mpin11, OUTPUT);
  
  //BLUETOOTH
  Serial.begin(38400); // Default communication rate of the Bluetooth module


  //WIFI
  Serial1.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  sendData("AT+RST\r\n", 2000, false); // resetare modul
  sendData("AT+CWMODE=2\r\n", 1000, false); // configurare ca
  //access point
  sendData("AT+CIFSR\r\n", 1000, DEBUG); // citește adresa IP
  sendData("AT+CWSAP?\r\n", 2000, DEBUG); // citește informația
  //SSID (nume rețea)
  sendData("AT+CIPMUX=1\r\n", 1000, false); // configurare
  //conexiuni multiple
  sendData("AT+CIPSERVER=1,80\r\n", 1000, false); // pornire
  //server pe port 80
  //sendData("AT+UART=115200,0,0\r\n", 2000, false);
}

void StartMotor (int m1, int m2, int forward, int speed)
{

 if (speed==0) // oprire
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
// Funcție de siguranță
// Execută oprire motoare, urmată de delay
void delayStopped(int ms)
{
 StartMotor (mpin00, mpin01, 0, 0);
 StartMotor (mpin10, mpin11, 0, 0);
 delay(500);
}

void loop() {



  //WIFI
  if (Serial1.available()) {
    if (Serial1.find("+IPD,")) {
      delay(500);
      int connectionId = Serial1.read() - 48; // functia
      //read() returnează valori zecimale ASCII
      // si caracterul ‘0’ are codul ASCII 48
      String webpage = "<h1>Hello World!</h1><a href=\"/l0\"><button>GREEN</button></a>";
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      webpage += "<a href=\"/l1\"><button>YELLOW</button></a>";
      webpage += "<a href=\"/l2\"><button>RED</button></a>";
      webpage += "<a href=\"/l3\"><button>BLUE</button></a>";

      if (readSensor() > 0) {
        webpage += "<h2>Millis:</h2>";
        webpage += readSensor();
      }
      cipSend += webpage.length();
      cipSend += "\r\n";
      sendData(cipSend, 100, DEBUG);
      sendData(webpage, 150, DEBUG);

      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; //se adaugă
      //identificatorul conexiunii
      closeCommand += "\r\n";
      sendData(closeCommand, 300, DEBUG);
    }
  }
}
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  Serial1.print(command); // trimite comanda la esp8266
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (Serial1.available()) {
      char c = Serial1.read(); // citește caracter următor
      response += c;
    }
  }
  if (response.indexOf("/l0") != -1) {
    digitalWrite(LED_BUILTIN, HIGH);
    //BLUETOOTH
    Serial.write('!');
    StartMotor (mpin00, mpin01, 0, 128);
 StartMotor (mpin10, mpin11, 0, 128);

 delay (500); // Cât timp e motorul pornit
 delayStopped(500); // Cât timp e oprit
  }
  if (response.indexOf("/l1") != -1) {
    digitalWrite(LED_BUILTIN, LOW);
    //BLUETOOTH
    Serial.write('@');
     StartMotor (mpin00, mpin01, 1, 128);
 StartMotor (mpin10, mpin11, 1, 128);

 delay (500);
 delayStopped(500);

  }

  if (response.indexOf("/l2") != -1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.write('#');
     StartMotor (mpin00, mpin01, 0, 128);
 StartMotor (mpin10, mpin11, 1, 128);

 delay (500);
 delayStopped(500);

  }

  if (response.indexOf("/l3") != -1)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.write('$');
     StartMotor (mpin00, mpin01, 1, 128);
 StartMotor (mpin10, mpin11, 0, 128);

 delay (500);
 delayStopped(500);
  }

  if (debug) {
    Serial.print(response);
  }

  return response;
}
unsigned long readSensor() {
  return millis();
}
