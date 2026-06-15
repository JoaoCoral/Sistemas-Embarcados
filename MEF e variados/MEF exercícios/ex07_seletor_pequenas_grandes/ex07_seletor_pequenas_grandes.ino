

const int pin_b1 = 2; 
const int pin_b0 = 3; 
const int pin_S1 = 4; 
const int pin_S2 = 5; 
const int pin_S3 = 6; 
const int pin_S4 = 7; 
const int pin_S5 = 8; 
const int pin_S6 = 9; 

const int pin_M1     = 10;
const int pin_C1     = 11; 
const int pin_C2     = 12; 
const int pin_ligada = 13;

int estado     = 0;
bool peca_grande = false;

void desligar_tudo() {
  digitalWrite(pin_M1,     LOW);
  digitalWrite(pin_C1,     LOW);
  digitalWrite(pin_C2,     LOW);
  digitalWrite(pin_ligada, LOW);
}

void setup() {
  pinMode(pin_b1, INPUT_PULLUP);
  pinMode(pin_b0, INPUT_PULLUP);
  pinMode(pin_S1, INPUT);
  pinMode(pin_S2, INPUT);
  pinMode(pin_S3, INPUT);
  pinMode(pin_S4, INPUT);
  pinMode(pin_S5, INPUT);
  pinMode(pin_S6, INPUT);

  pinMode(pin_M1,     OUTPUT);
  pinMode(pin_C1,     OUTPUT);
  pinMode(pin_C2,     OUTPUT);
  pinMode(pin_ligada, OUTPUT);

  Serial.begin(9600);
  desligar_tudo();
}

void loop() {
  bool start = (digitalRead(pin_b1) == LOW);
  bool stop  = (digitalRead(pin_b0) == LOW);
  bool S1    = digitalRead(pin_S1);
  bool S2    = digitalRead(pin_S2);
  bool S3    = digitalRead(pin_S3);
  bool S4    = digitalRead(pin_S4);
  bool S5    = digitalRead(pin_S5);
  bool S6    = digitalRead(pin_S6);

  if (stop) {
    desligar_tudo();
    estado = 0;
    return;
  }

  switch (estado) {

    case 0: 
      desligar_tudo();
      if (start) {
        digitalWrite(pin_ligada, HIGH);
        estado = 1;
        Serial.println("Ciclo iniciado.");
      }
      break;

    case 1: 
      digitalWrite(pin_M1, HIGH);
      digitalWrite(pin_C1, HIGH);
      estado = 2;
      Serial.println("M1 ligado, C1 avancado.");
      break;

    case 2: 
      if (S3 == HIGH) {
        peca_grande = true;
        Serial.println("Peca GRANDE detectada.");
        estado = 3;
      } else if (S2 == HIGH && S3 == LOW) {
        peca_grande = false;
        Serial.println("Peca PEQUENA detectada.");
        estado = 3;
      }
      break;

    case 3:
      if (peca_grande) {
        if (S6 == HIGH) {
          estado = 5;
        }
      } else {
        if (S4 == HIGH) {
          digitalWrite(pin_C2, HIGH);
          Serial.println("C2 avancado: desviando peca pequena.");
          estado = 4;
        }
      }
      break;

    case 4:
      if (S5 == HIGH) {
        estado = 5;
      }
      break;

    case 5:
      digitalWrite(pin_M1,     LOW);
      digitalWrite(pin_C1,     LOW);
      digitalWrite(pin_C2,     LOW);
      digitalWrite(pin_ligada, LOW);
      Serial.println("Ciclo finalizado.");
      estado = 0;
      break;
  }
}
