int LED_PIN = D0; // connect to LED (pin 4 on sensor)
int PM_PIN = A0; // connect to Vo (pin 2 on sensor)
int TEMP_PIN = A1; // connect to middle pin on temperature sensor

double pmVoltage;
double dustDensity;
double tempVoltage;
double temperature;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(PM_PIN, INPUT);
    pinMode(TEMP_PIN, INPUT);

    Spark.variable("pmVoltage", &pmVoltage, DOUBLE);
    Spark.variable("dustdensity", &dustDensity, DOUBLE);
    Spark.variable("temperature", &temperature, DOUBLE);
}

void loop() {
    digitalWrite(LED_PIN, LOW);         // start of pulse
    delayMicroseconds(280);             // time until sample is ready
    int raw = analogRead(PM_PIN);       // sample (raw is value from 0-4095, mapping from 0-3.3V)
    delayMicroseconds(40);              // time until end of pulse
    digitalWrite(LED_PIN, HIGH);        // end of pulse
    delayMicroseconds(9680);            // time between pulses

    pmVoltage = raw * 3.3 / 4095.0;     // 3.3V across 0-4095

    // Needs fixing
    // equation taken from http://www.dfrobot.com/wiki/index.php/Sharp_GP2Y1010AU
    dustDensity = 0.17 * pmVoltage - 0.1;

    tempVoltage = analogRead(TEMP_PIN) * 3.3 / 4095.0;
    temperature = 25 + (tempVoltage - .75) * 100; // 25C at 750mV, 10mV/C
}
