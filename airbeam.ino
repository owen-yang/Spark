#include <DHT.h>
#include <stdlib.h>
//#include <string.h>

#define DHTPIN 9
#define DHTTYPE DHT22   // DHT 22  (AM2302)

int pin = 8;
int humi, cel, fah, kelv; 
unsigned long analogcount;
double analogtotal;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 1000;
unsigned long lowpulseoccupancy = 0;
unsigned long analogvalue = 0;
double ratio = 0;
double concentrationAV = 0;
DHT dht(DHTPIN, DHTTYPE);

#define LEDPIN 13
#define BT_BUF_LEN 10
char bt_buffer[BT_BUF_LEN];
int8_t bt_connected = -1; // 0 = not connected (but been connected), 1 == connected, -1 == never been connected

void setup() {
  Serial.begin(115200);
  bt_buffer[0] = 0;
  pinMode(8,INPUT);
  pinMode(LEDPIN, OUTPUT);
  starttime = millis();
}

void loop() {
  unsigned long int start = millis(); // The main block takes at least 1.5 seconds!
  humi = dht.readHumidity();
  cel = dht.readTemperature();
  fah = ((cel * 9)/5) + 32;
  analogvalue = analogvalue + analogRead(0);
  analogcount++;
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;
  Serial.print(F("Took ms: "));
  Serial.println(millis() - start);
  if ((millis()-starttime) > sampletime_ms)
  {
    starttime = millis(); // moved to create more regular periods.
    update_bt_status();
    
    Serial.println();
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    analogvalue = analogvalue/analogcount;
    analogtotal = (analogvalue*5.0)/1024;
   
    concentrationAV = (1244.4*pow(analogtotal,3) - 2961.9*pow(analogtotal,2) + 6007.9*analogtotal - 257.14);
    if(concentrationAV < 0){
      concentrationAV = 0;
    }
    Serial.print(fah);
    Serial.print(F("F "));
    Serial.print(cel);
    Serial.print(F("C "));
    Serial.print(humi);
    Serial.print(F("RH "));
    Serial.print(F(" Analog Value: "));
    Serial.print(analogvalue);
    
    analogvalue = 0;
    analogcount = 0;
    lowpulseoccupancy = 0;
    Serial.println();
    
  }
}
