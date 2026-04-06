/* ************************************************************************

Colégio SATC
Curso Técnico em Mecatrônica
Disciplina: Sistemas Embarcados
Turma: 3191
Professor: Marcos Antonio Jeremias Coelho

Programa: Leitura de Entrada Analógica e Saída PWM

Autor: Marcos Antonio Jeremias Coelho
Data: 22/05/2023
Versão: 1.0

************************************************************************ */


const int analogPin = A0; 
const int led = 9;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int valor = analogRead(analogPin);
  Serial.println(valor);
  valor = map(valor, 0, 1023, 0, 255);
  analogWrite(led, valor);
}
