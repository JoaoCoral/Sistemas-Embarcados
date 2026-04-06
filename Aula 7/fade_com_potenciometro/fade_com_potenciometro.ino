const int pin_sensor = A0;
const int led_1 = 9;
int valor_sensor = 0;


void setup() {
  // put your setup code here, to run once:
pinMode (pin_sensor, INPUT);
pinMode (led_1, OUTPUT);
Serial.begin (115200);
}

void loop() {
  // put your main code here, to run repeatedly:
valor_sensor = analogRead (pin_sensor)/ 4;
analogWrite (led_1, valor_sensor);
delay (10);
}
