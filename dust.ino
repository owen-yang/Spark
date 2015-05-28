int LED_PIN = D0; // connect to LED (pin 4 on sensor)
int PM_PIN = A0; // connect to Vo (pin 2 on sensor)
int LT_PIN = D7;

double pmVoltage;
double dustDensity;
int dust ();

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(LT_PIN, OUTPUT);
    pinMode(PM_PIN, INPUT);

    Spark.variable("pmVoltage", &pmVoltage, DOUBLE);
    Spark.variable("dustdensity", &dustDensity, DOUBLE);
    Spark.function("dust",dust);
}

void loop() {
}

int dust (String s) {
    digitalWrite(LT_PIN, HIGH);
    for (int j = 0; j < 10; j++ ){
        int n = 300;
        double totalVoltage = 0;
        unsigned long start = micros();
        for (int i = 0; i < n; i++) {         // 320µs per loop
            digitalWrite(LED_PIN, LOW);         // start of pulse
            delayMicroseconds(280);             // time until sample is ready
            int raw = analogRead(PM_PIN);       // sample (raw is value from 0-4095, mapping from 0-3.3V)
            delayMicroseconds(40);              // time until end of pulse
            digitalWrite(LED_PIN, HIGH);        // end of pulse
            totalVoltage += raw * 3.3 / 4095.0; // voltage mapped and added
        }
        unsigned long finish = micros();
        pmVoltage = (totalVoltage/n);
        dustDensity = 0.17 * pmVoltage - 0.1;
        Spark.publish("dust_sense", "{\"dust\":"+String(dustDensity)+",\"voltage\":"+String(pmVoltage)+",\"ts\":"+millis()+"}" );
        delay(1000);
    }
    digitalWrite(LT_PIN, LOW);
    return 1;
}
