/* ************************************************************************

Colégio SATC
Curso Técnico em Mecatrônica
Disciplina: Sistemas Embarcados
Turma: 3191
Aluno: João Pedro Coral

Programa: Debouncer: limitar o tempo de execução do arduino

Data: 23/03/2026
Versão: 1.0

************************************************************************ */

// Exemplo simples de Debounce no Arduino
const int pinoBotao = 2;
const int pinoLed = 5;

int estadoBotao;
int ultimoEstadoBotao = LOW; // Estado anterior
unsigned long ultimoTempoDebounce = 0;
unsigned long delayDebounce = 50; // Tempo de espera (50ms)

void setup() {
  pinMode(pinoBotao, INPUT);
  pinMode(pinoLed, OUTPUT);
}

void loop() {
  int leitura = digitalRead(pinoBotao);

  // Se o botão mudou (por ruído ou pressão)
  if (leitura != ultimoEstadoBotao) {
    ultimoTempoDebounce = millis(); // Reseta o timer
  }

  // Se passou tempo suficiente, a leitura é válida
  if ((millis() - ultimoTempoDebounce) > delayDebounce) {
    if (leitura != estadoBotao) {
      estadoBotao = leitura;
      if (estadoBotao == HIGH) {
        // Ação: Ligar/Desligar LED
        digitalWrite(pinoLed, !digitalRead(pinoLed));
      }
    }
  }

  ultimoEstadoBotao = leitura;
}