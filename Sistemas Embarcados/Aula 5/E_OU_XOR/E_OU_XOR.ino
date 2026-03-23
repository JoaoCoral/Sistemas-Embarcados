/* ************************************************************************

Colégio SATC
Curso Técnico em Mecatrônica
Disciplina: Sistemas Embarcados
Turma: 3191
Aluno: João Pedro Coral

Programa: Ligação de leds com AND, OR e XOR

Data: 23/03/2026
Versão: 1.0

************************************************************************ */
const int led_vermelho = 5;			//
const int led_verde = 6;			//
const int led_amarelo = 7;			//
char led;					//
const int pin_botaoA = 2;			//
const int pin_botaoB = 3;
bool botaoA = false;				//
bool botaoB = false;				//

void setup() {
  pinMode(led_vermelho, OUTPUT);		//
  pinMode(led_verde, OUTPUT);			//
  pinMode(led_amarelo, OUTPUT);			//
  pinMode(pin_botaoA, INPUT_PULLUP);			// definindo botao A com pull-up interno
  pinMode(pin_botaoB, INPUT_PULLUP);			// definindo botao B com pull-up interno
  Serial.begin(115200); 			// Definição da velocide de transmissão em 115200 bps
}

void loop() {
  botaoA = digitalRead(pin_botaoA);			
  botaoB = digitalRead(pin_botaoB);
  {
   if (botaoA == 0 && botaoB == 0)	{
    digitalWrite (led_vermelho, HIGH);
   }
   else digitalWrite (led_vermelho, LOW);
   }

   if (botaoA == 0 || botaoB == 0)	{
    digitalWrite (led_verde, HIGH);
   }
   else digitalWrite (led_verde, LOW);
  
  
   if (botaoA == 0 ^ botaoB == 0)	{
    digitalWrite (led_amarelo, HIGH);
   }
   else digitalWrite (led_amarelo, LOW);
  }

