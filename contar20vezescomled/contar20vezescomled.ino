int led = 9;




void setup() {
  Serial.begin (9600);
  pinMode(led, OUTPUT);
    for (int i = 0; i < 20; i++){
    digitalWrite (led, HIGH);
      delay (500);
    digitalWrite (led, LOW);
      delay (500);
      Serial.println (i);
  } 
}

void loop()
{

  
}
