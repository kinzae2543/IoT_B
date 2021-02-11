#define USE_ARDUINO_INTERRUPTS true
#define BLYNK_PRINT Serial
#define ESP8266_BAUD 9600

//library
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
#include <PulseSensorPlayground.h>

//fitur yang diambil dari library
BlynkTimer timer;
SoftwareSerial EspSerial(3, 4); // RX=3, TX=4
ESP8266 wifi(&EspSerial);
PulseSensorPlayground pulseSensor;

//mengkoneksikan ke wifi dan aplikasi Blynk
char auth[] = "1P8t_vBmgqy5ENECUK5HEMMGjSHg1P72";
//char ssid[] = "ridwanjournal";
//char pass[] = "eac10602aacd";
char ssid[] = "Jamur 6";
char pass[] = "poplopklop123";

//threshold digunakan agar sensor dapat membedakan sebuah sinyal yang diterima
int Threshold = 550;

const int PulseWire = 0; //AO
const int BUZZER6 = 6; //D6
const int LED8 = 8; //D8
const int LED10 = 10; //D10

void setup()
{
  //baudrate yang dipakai
  Serial.begin(9600);
  delay(10);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  //Blynk.begin(auth, wifi, ssid, pass);
  Blynk.begin(auth, wifi, ssid, pass, 8080);
  
  pinMode(BUZZER6, OUTPUT);
  
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold);   
  if (pulseSensor.begin()) {
    Serial.println("Objek yang menghasilkan detak jantung terdeteksi!");
  }

  timer.setInterval(1000, sendUptime);
}

void sendUptime()
{
  //convert nilai hasil sensor menjadi bpm
  int BPM = pulseSensor.getBeatsPerMinute();
  if(pulseSensor.sawStartOfBeat()) {
    if(BPM < 40 || BPM > 100)
    {
      Serial.println("Detak jantung dalam keadaan tidak normal!");
      pulseSensor.blinkOnPulse(LED10);
      tone(BUZZER6, 1000);
      Serial.print("BPM: "); 
      Serial.println(BPM);
      Blynk.virtualWrite(A0,BPM);
    }
    else {
      Serial.println("Detak jantung dalam keadaan normal!");
      pulseSensor.blinkOnPulse(LED8);
      Serial.print("BPM: "); 
      Serial.println(BPM);
      Blynk.virtualWrite(A0,BPM);
    }   
  }  
  delay(200);
  noTone(BUZZER6);
  digitalWrite(LED10, LOW);
  digitalWrite(LED8, LOW); 
}

void loop()
{
  Blynk.run();
  timer.run();
}
