int pin_sensor = A0;
int valor_sensor = 0;





void setup() {
  // put your setup code here, to run once:
  pinMode (pin_sensor, INPUT);
 Serial.begin (115200);
}

void loop() {
  // put your main code here, to run repeatedly:
valor_sensor = analogRead (pin_sensor);
Serial.print ("Valor analogico =");
Serial.println (valor_sensor);
delay (200);
}
