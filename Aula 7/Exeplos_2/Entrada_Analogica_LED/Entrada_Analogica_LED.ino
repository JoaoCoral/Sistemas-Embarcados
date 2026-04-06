/* ************************************************************************

Colégio SATC
Curso Técnico em Mecatrônica
Disciplina: Sistemas Embarcados
Turma: 3191
Professor: Marcos Antonio Jeremias Coelho

Programa: Leitura de Entrada Analógica controlando LED

Autor: Marcos Antonio Jeremias Coelho
Data: 22/05/2023
Versão: 1.0

************************************************************************ */

const int analogPin = A0; 
const int led = 6;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int val = analogRead(analogPin);
  Serial.println(val);
  digitalWrite(led, HIGH);
  delay(val);
  digitalWrite(led, LOW);
  delay(val);
}
