int leds[5] = {8, 9, 10, 11, 12};




void setup() {
  Serial.begin (9600);
    for (int i = 0; i < 5; i++) {
     pinMode(leds[i], OUTPUT);
  } 
}

void loop()
{
  for (int i = 0; i < 5; i++) {
    digitalWrite (leds [i], HIGH);
    delay (500);
    digitalWrite (leds [i], LOW);
    Serial.println (i);
  }  
}
