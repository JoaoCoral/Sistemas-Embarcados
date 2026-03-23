/* ************************************************************************

Colégio SATC
Curso Técnico em Mecatrônica
Disciplina: Sistemas Embarcados
Turma: 3191
Aluno: João Pedro Coral

Programa: Botão com Pull-up e envio serial

Data: 23/03/2026
Versão: 1.0

************************************************************************ */

const int pin_botao = 2;			//
boolean botao = false;				//

void setup() {
  pinMode(pin_botao, INPUT_PULLUP);			//
  Serial.begin(115200); 			// Definição da velocide de transmissão em 115200 bps
}

void loop() {
  botao = digitalRead(pin_botao);			//
  if (botao == LOW) {				//
    Serial.println("Botao pressionado!!!");	//
  }
}
