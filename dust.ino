#include "/libraries/DHT/DHT.h"

int LED_PIN = D0; // connect to LED (pin 4 on sensor)
int PM_PIN = A0;  // connect to Vo (pin 2 on sensor)
int LT_PIN = D7;
int FAN_PIN = D1;

double pmVoltage;
double dustDensity;
double referenceVoltage;
double beta = 1.0;
double alpha = 0.6;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(LT_PIN, OUTPUT);
    pinMode(PM_PIN, INPUT);
    pinMode(FAN_PIN, OUTPUT);
    
    Spark.variable("pmVoltage", &pmVoltage, DOUBLE);
    Spark.variable("dustdensity", &dustDensity, DOUBLE);
    Spark.variable("referenceVoltage", &referenceVoltage, DOUBLE);
    Spark.variable("beta", &beta, DOUBLE);
    Spark.function("dust",dust);
    Spark.function("calibrate",calibrate);
    Spark.function("fan",turn_on_fan);
}

void loop() {
}

int turn_on_fan (String s) {
    if (s == "1") {
        digitalWrite(FAN_PIN, HIGH);
        return 1;
    } else if (s == "0") {
        digitalWrite(FAN_PIN, LOW);
        return 0;
    } else {
        return -1;
    }
}

int dust (String s) {
    digitalWrite(LT_PIN, HIGH);
    for (int j = 0; j < 10; j++ ){
        int n = 300;
        double totalVoltage = 0;
        for (int i = 0; i < n; i++) {           // 320µs per loop
            digitalWrite(LED_PIN, HIGH);         // start of pulse
            delayMicroseconds(280);             // time until sample is ready
            int raw = analogRead(PM_PIN);       // sample (raw is value from 0-4095, mapping from 0-3.3V)
            delayMicroseconds(40);              // time until end of pulse
            digitalWrite(LED_PIN, LOW);        // end of pulse
            totalVoltage += raw * 3.3 / 4095.0; // voltage mapped and added
        }
        pmVoltage = (totalVoltage/n);
        dustDensity = beta * alpha * ( (pmVoltage-referenceVoltage) * 100);
        Spark.publish("dust_sense", "{\"pm25\":"+String(dustDensity)+",\"voltage\":"+String(pmVoltage)+",\"ts\":"+millis()+"}" );
        delay(1000);
    }
    digitalWrite(LT_PIN, LOW);
    return 1;
}


// make sure the fan is off
int calibrate (String s) {
    int h = s.toInt();
    h = (h>50) ? 1.0 - ( 0.01467 * ( h - 50 ) ) : 1.0;
    alpha = 0.6;

    double avgReferenceVoltage = 0.0;
    int m = 5;
    for(int j = 0; j < m; j++) {
        int n = 300;
        double totalVoltage = 0.0;
        for (int i = 0; i < n; i++) {           // 320µs per loop
            digitalWrite(LED_PIN, HIGH);         // start of pulse
            delayMicroseconds(280);             // time until sample is ready
            int raw = analogRead(PM_PIN);       // sample (raw is value from 0-4095, mapping from 0-3.3V)
            delayMicroseconds(40);              // time until end of pulse
            digitalWrite(LED_PIN, LOW);        // end of pulse
            totalVoltage += raw * 3.3 / 4095.0; // voltage mapped and added
        }
        avgReferenceVoltage += (totalVoltage/n);
        delay(500);
    }
    referenceVoltage = (avgReferenceVoltage / m);

    Spark.publish("calibration", "{\"beta\":"+String(beta)+",\"Vs\":"+String(referenceVoltage)+",\"h\":"+String(h)+"}");
    return 1;
}
