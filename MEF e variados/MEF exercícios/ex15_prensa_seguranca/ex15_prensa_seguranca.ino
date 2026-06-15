

const int pin_A = 2;  
const int pin_B = 3;  
const int pin_R = 4;  

int estado = 0;
unsigned long tempo_ant  = 0;
unsigned long tempo_atual = 0;

void setup() {
  pinMode(pin_A, INPUT_PULLUP);
  pinMode(pin_B, INPUT_PULLUP);
  pinMode(pin_R, OUTPUT);
  Serial.begin(9600);
  digitalWrite(pin_R, LOW);
}

void loop() {
  tempo_atual = millis();

  bool A = (digitalRead(pin_A) == LOW);
  bool B = (digitalRead(pin_B) == LOW);

  switch (estado) {

    case 0: 
      digitalWrite(pin_R, LOW);
      if (A && B) {
        digitalWrite(pin_R, HIGH);
        Serial.println("Prensa ativada.");
        estado = 3;
      } else if (A && !B) {
        tempo_ant = tempo_atual;
        Serial.println("Op.1 acionou. Aguardando Op.2 (3s).");
        estado = 1;
      } else if (!A && B) {
        tempo_ant = tempo_atual;
        Serial.println("Op.2 acionou. Aguardando Op.1 (3s).");
        estado = 2;
      }
      break;

    case 1:
      if (A && B) {
        digitalWrite(pin_R, HIGH);
        Serial.println("Prensa ativada.");
        estado = 3;
      } else if (!A) {
        Serial.println("Op.1 soltou. Reiniciar manobra.");
        estado = 0;
      } else if (tempo_atual - tempo_ant > 3000) {
        Serial.println("Timeout: Op.2 nao acionou em 3s. Repetir manobra.");
        estado = 0;
      }
      break;

    case 2: 
      if (A && B) {
        digitalWrite(pin_R, HIGH);
        Serial.println("Prensa ativada.");
        estado = 3;
      } else if (!B) {
        Serial.println("Op.2 soltou. Reiniciar manobra.");
        estado = 0;
      } else if (tempo_atual - tempo_ant > 3000) {
        Serial.println("Timeout: Op.1 nao acionou em 3s. Repetir manobra.");
        estado = 0;
      }
      break;

    case 3: 
      if (!A && !B) {
        digitalWrite(pin_R, LOW);
        Serial.println("Ambos soltaram. Prensa desativada.");
        estado = 0;
      } else if (!A) {
        tempo_ant = tempo_atual;
        Serial.println("Op.1 soltou. 3s para recolocar maos.");
        estado = 4;
      } else if (!B) {
        tempo_ant = tempo_atual;
        Serial.println("Op.2 soltou. 3s para recolocar maos.");
        estado = 5;
      }
      break;

    case 4:
      if (A && B) {
        // op.1 recolocou a mao dentro de 3s
        Serial.println("Op.1 recolocou. Prensa continua.");
        estado = 3;
      } else if (tempo_atual - tempo_ant > 3000) {
        digitalWrite(pin_R, LOW);
        Serial.println("Timeout Op.1. Prensa desativada. Repetir manobra.");
        estado = 0;
      }
      break;

    case 5:
      if (A && B) {
        Serial.println("Op.2 recolocou. Prensa continua.");
        estado = 3;
      } else if (tempo_atual - tempo_ant > 3000) {
        digitalWrite(pin_R, LOW);
        Serial.println("Timeout Op.2. Prensa desativada. Repetir manobra.");
        estado = 0;
      }
      break;
  }
}
