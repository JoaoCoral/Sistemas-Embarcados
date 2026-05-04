int leds[3] = {8, 9, 10};
int intervalo[3] = {6000, 2000, 4000};


void setup() {
  Serial.begin (9600);
  for (int i = 0; i < 3; i++) {
  pinMode (leds[i], OUTPUT);
    
  }

}

void loop() {
   for (int i = 0; i < 3; i++) {
   digitalWrite (leds[i], HIGH);
   delay (intervalo[i]);
   digitalWrite (leds[i], LOW);
  }

}
