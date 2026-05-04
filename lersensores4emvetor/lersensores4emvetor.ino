int sensores[4] = {A0, A1, A2, A3};




void setup() {
  Serial.begin (9600);
    for (int i = 0; i < 4; i++) {
  } 
}

void loop()
{
  for (int i = 0; i < 4; i++) {
    Serial.println (analogRead (sensores[i]));
    delay (1000);
  }  
}
