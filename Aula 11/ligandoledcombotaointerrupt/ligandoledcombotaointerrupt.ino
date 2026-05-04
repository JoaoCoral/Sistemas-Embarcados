const int led = 9;
const int interruptpin1 = 2;
bool state = LOW;



void setup() {
  pinMode (led, OUTPUT);
  pinMode (interruptpin, INPUT_PULLUP);
  attachInterrupt (digitalPinToInterrupt(interruptpin), blink, CHANGE);
}

void loop() {
  digitalWrite (led, state);

}

void blink() {
 
 state = !state;

}