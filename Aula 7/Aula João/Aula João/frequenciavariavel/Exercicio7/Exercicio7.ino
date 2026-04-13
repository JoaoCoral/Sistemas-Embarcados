#define saidaLed   9

const long int intervalo = 200;
int tempoAtual = 0, tempoAnterior = 0;
int controleLed = 0;
long int saidaAnalogica = 0, entradaAnalogica = 0;
float tempoLed = 0;
bool auxPulso = 0;

void setup() {
  // put your setup code here, to run once:
  //pinMode(botaoPulso, INPUT_PULLUP);
  pinMode(saidaLed, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  entradaAnalogica = analogRead(A0);
  tempoLed = ((1.99*entradaAnalogica) - 2138.77)/-1023;

  tempoAtual = millis(); // Obtém o tempo atual

  if (tempoAtual - tempoAnterior >= ((int)tempoLed*1000)) {
    tempoAnterior = tempoAtual; // Salva o último momento em que o LED piscou
    digitalWrite(saidaLed, !digitalRead(saidaLed));
  }
 /* Serial.print("Controle LED: ");
  Serial.println(controleLed);
  Serial.print("Aux Pulso: ");
  Serial.println(auxPulso);*/
}
