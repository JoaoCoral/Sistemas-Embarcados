const int led = 9;
const int interruptpin1 = 2;
const int interruptpin2 = 3;
bool state = LOW;



void setup() {
  pinMode (led, OUTPUT);
  pinMode (interruptpin2, INPUT_PULLUP);
  pinMode (interruptpin1, INPUT_PULLUP);
  attachInterrupt (digitalPinToInterrupt(interruptpin1), on, FALLING);
  attachInterrupt (digitalPinToInterrupt(interruptpin2), off, FALLING);
}

void loop() {
  digitalWrite (led, state);

}

void on() {
 
 state = 1;

}

void off() {
 
 state = 0;

}