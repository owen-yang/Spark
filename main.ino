int LED_PIN = A0; // connect to LED (pin 3 on sensor)
int SENSOR_PIN = A1; // connect to Vo (pin 5 on sensor)

double dustDensity;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(SENSOR_PIN, INPUT);

    Spark.variable("dustdensity", &dustDensity, DOUBLE);
}

void loop() {
    digitalWrite(LED_PIN, HIGH);        // start of pulse
    delayMicroseconds(280);             // time until sample is ready
    int raw = analogRead(SENSOR_PIN);   // sample (raw is value from 0-4095, mapping from 0-3.3V)
    delayMicroseconds(40);              // time until end of pulse
    digitalWrite(LED_PIN, LOW);         // end of pulse
    delayMicroseconds(9680);            // time between pulses

    double voltage = raw * 3.3 / 4096.0; // 3.3V / 4096bits

    // equation taken from http://www.dfrobot.com/wiki/index.php/Sharp_GP2Y1010AU
    dustDensity = 0.17 * voltage - 0.1;
}
