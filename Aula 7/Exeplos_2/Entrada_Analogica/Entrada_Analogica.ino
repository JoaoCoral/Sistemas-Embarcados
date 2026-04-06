/* ************************************************************************

Colégio SATC
Curso Técnico em Mecatrônica
Disciplina: Sistemas Embarcados
Turma: 3191
Professor: Marcos Antonio Jeremias Coelho

Programa: Leitura de Entrada Analógica

Autor: Marcos Antonio Jeremias Coelho
Data: 22/05/2023
Versão: 1.0

************************************************************************ */

const int analogPin = A0; 

int valor = 0;     
float valor2 = 0;     

void setup() {
  Serial.begin(9600);
}

void loop() {
  valor = analogRead(analogPin);
  valor2 = 5 * float(valor) / 1023;
  Serial.println(valor);
  Serial.println(valor2);
}
