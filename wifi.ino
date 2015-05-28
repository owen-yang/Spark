int led = D0;
int led2 = D7;
int state = 0;

void setup() {
    Spark.function("led", ledControl);
    Spark.variable("state", &state, INT);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    pinMode(led2, OUTPUT);
    digitalWrite(led2, LOW);
}

void loop() {

}

int ledControl(String command){
   // find out the state of the led
   if(command == "HIGH") state = 1;
   else if(command == "LOW") state = 0;
   else return -1;

   delay(100);
   digitalWrite(led2, 0);
   delay(100);
   digitalWrite(led2, 1);
   delay(100);
   digitalWrite(led2, 0);
   delay(100);
   digitalWrite(led2, 1);
   delay(100);
   digitalWrite(led2, 0);
   delay(100);
   digitalWrite(led2, 1);

   delay(1000);
   // write to the appropriate pin
   digitalWrite(led, state);
   digitalWrite(led2, state);
   return state;
}

// POST /v1/devices/domcore0/led
// curl https://api.spark.io/v1/devices/domcore0/led -d access_token=f9cf70a78e732b1c1067c2d8c10de165d2443739 -d params=HIGH

// GET /v1/devices/domcore0/state
// curl -G https://api.spark.io/v1/devices/domcore0/state -d access_token=f9cf70a78e732b1c1067c2d8c10de165d2443739
