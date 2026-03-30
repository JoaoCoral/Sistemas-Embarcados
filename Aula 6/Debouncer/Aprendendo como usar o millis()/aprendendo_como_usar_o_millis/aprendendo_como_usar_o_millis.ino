int ledverde = 8;
int ledamarelo = 9;
int ledvermelho = 10;
int ledazul = 11;
unsigned long tempo_anterior = 0;
int intervalo = 1000;
bool estado = 0;

void setup() {
pinMode (8,OUTPUT);
pinMode (9, OUTPUT);
pinMode (10, OUTPUT);
pinMode (11, OUTPUT);
Serial.begin (115200);


}

void loop() {
unsigned long tempo_atual = millis();
if (tempo_atual - tempo_anterior >= intervalo) {
  estado = !estado;
  digitalWrite (8, estado);
  tempo_anterior = tempo_atual;
  Serial.println ("1 segundo");
  }
}
