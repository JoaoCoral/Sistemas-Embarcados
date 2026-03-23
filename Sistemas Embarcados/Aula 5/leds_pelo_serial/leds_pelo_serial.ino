/* ************************************************************************

Colégio SATC
Curso Técnico em Mecatrônica
Disciplina: Sistemas Embarcados
Turma: 3191
Aluno: João Pedro Coral

Programa: Ligando LEDs pelo Serial

Data: 23/03/2026
Versão: 1.0

************************************************************************ */

const int led_vermelho = 5;			//
const int led_verde = 6;			//
const int led_amarelo = 7;			//
char led;					//

void setup() {
  pinMode(led_vermelho, OUTPUT);		//
  pinMode(led_verde, OUTPUT);			//
  pinMode(led_amarelo, OUTPUT);			//
  Serial.begin(115200); 			// Definição da velocide de transmissão em 115200 bps
}

void loop() {
  if (Serial.available()) {			//
    led = Serial.read();			//
    if(led == 'R') { 				// Led vermelho - red
      digitalWrite(led_vermelho, HIGH); 	// Acende led vermelho
    }
    if (led == 'G') { 				// Led verde - green
      digitalWrite(led_verde, HIGH); 		// Acende led verde
    }
    if (led == 'Y') { 				// Led amarelo - yellow
      digitalWrite(led_amarelo, HIGH); 		// Acende led amarelo
    }
    if(led == 'r') { 				// Led vermelho - red
      digitalWrite(led_vermelho, LOW); 	// Apaga led vermelho
    }
    if (led == 'g') { 				// Led verde - green
      digitalWrite(led_verde, LOW); 		// Apaga led verde
    }
    if (led == 'y') { 				// Led amarelo - yellow
      digitalWrite(led_amarelo, LOW); 		// Apaga led amarelo
    }
    if (led == 'T') { 				// Led T - todos eles
      digitalWrite(led_vermelho, HIGH);
      digitalWrite(led_verde, HIGH);
      digitalWrite(led_amarelo, HIGH); 		// Acende todos os leds
    }
    if (led == 't') { 				// Led T - todos eles
      digitalWrite(led_vermelho, LOW);
      digitalWrite(led_verde, LOW);
      digitalWrite(led_amarelo, LOW); 		// Apaga todos os leds
    }
  }
}
