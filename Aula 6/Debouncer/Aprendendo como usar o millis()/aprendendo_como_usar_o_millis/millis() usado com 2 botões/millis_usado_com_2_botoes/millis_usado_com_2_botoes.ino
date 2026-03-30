const int pin_botaoA = 2;
const int pin_ledA = 8;
int estado_ledA = LOW;
int estado_botaoA;
int ultimo_estado_botaoA = HIGH;
unsigned long tempo_debounceA = 0;
const int pin_botaoB = 3;
const int pin_ledB = 9;
int estado_ledB = LOW;
int estado_botaoB;
int ultimo_estado_botaoB = HIGH;
unsigned long tempo_debounceB = 0;
unsigned long atraso = 50;

void setup() {
pinMode(pin_botaoA, INPUT_PULLUP);
pinMode(pin_ledA, OUTPUT);
digitalWrite(pin_ledA, estado_ledA);
pinMode(pin_botaoB, INPUT_PULLUP);
pinMode(pin_ledB, OUTPUT);
digitalWrite(pin_ledB, estado_ledB);


}

void loop() 
// BOTAO A
{
int leituraA = digitalRead(pin_botaoA);
if (leituraA != ultimo_estado_botaoA) {
  tempo_debounceA = millis();
}
if ((millis() - tempo_debounceA) > atraso)
{
  if (leituraA != estado_botaoA) {
    estado_botaoA = leituraA;
    if (estado_botaoA == LOW) {
      estado_ledA = !estado_ledA;
    }
  }
}

{
int leituraB = digitalRead(pin_botaoB);
if (leituraB != ultimo_estado_botaoB) {
  tempo_debounceB = millis();
}
if ((millis() - tempo_debounceB) > atraso)
{
  if (leituraB != estado_botaoB) {
    estado_botaoB = leituraB;
    if (estado_botaoB == LOW) {
      estado_ledB = !estado_ledB;
    }
  }
}
digitalWrite(pin_ledA, estado_ledA);
ultimo_estado_botaoA = leituraA;
digitalWrite(pin_ledB, estado_ledB);
ultimo_estado_botaoB = leituraB;
}
}