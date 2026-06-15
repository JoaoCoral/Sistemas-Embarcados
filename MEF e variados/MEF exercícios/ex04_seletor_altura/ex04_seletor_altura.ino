

const int pin_b1 = 2;  
const int pin_b0 = 3;  
const int pin_S1 = 4;  
const int pin_S2 = 5; 
const int pin_S3 = 6;  
const int pin_S4 = 7;  

const int pin_M1     = 8;  
const int pin_Q2     = 9;  
const int pin_ligada = 10; 

int estado = 0;
unsigned long tempo_ant  = 0;
unsigned long tempo_atual = 0;
bool peca_alta = false;

void desligar_tudo() {
  digitalWrite(pin_M1,     LOW);
  digitalWrite(pin_Q2,     LOW);
  digitalWrite(pin_ligada, LOW);
}

void setup() {
  pinMode(pin_b1, INPUT_PULLUP);
  pinMode(pin_b0, INPUT_PULLUP);
  pinMode(pin_S1, INPUT);
  pinMode(pin_S2, INPUT);
  pinMode(pin_S3, INPUT);
  pinMode(pin_S4, INPUT);

  pinMode(pin_M1,     OUTPUT);
  pinMode(pin_Q2,     OUTPUT);
  pinMode(pin_ligada, OUTPUT);

  Serial.begin(9600);
  desligar_tudo();
}

void loop() {
  tempo_atual = millis();

  bool start = (digitalRead(pin_b1) == LOW);
  bool stop  = (digitalRead(pin_b0) == LOW);
  bool S1    = digitalRead(pin_S1); 
  bool S2    = digitalRead(pin_S2); 
  bool S3    = digitalRead(pin_S3); 
  bool S4    = digitalRead(pin_S4); 

  if (stop) {
    desligar_tudo();
    estado = 0;
    return;
  }

  switch (estado) {

    case 0: 
      desligar_tudo();
      if (start) {
        estado = 1;
        Serial.println("Ciclo iniciado.");
      }
      break;

    case 1: 
      digitalWrite(pin_M1,     HIGH);
      digitalWrite(pin_ligada, HIGH);
      tempo_ant = tempo_atual;
      estado = 2;
      Serial.println("Esteira ligada. Aguardando peca...");
      break;

    case 2: 
      if (tempo_atual - tempo_ant >= 2000) {
        estado = 3;
      }
      break;

    case 3: 
      if (S4 == HIGH) {
        peca_alta = true;
        Serial.println("Peca ALTA detectada.");
      } else if (S3 == HIGH) {
        peca_alta = false;
        Serial.println("Peca BAIXA detectada.");
      }
      estado = 4;
      break;

    case 4: 
      if (peca_alta) {
        digitalWrite(pin_Q2, LOW);
        if (S1 == HIGH) {
          estado = 5;
        }
      } else {
        digitalWrite(pin_Q2, HIGH);
        if (S2 == HIGH) {
          estado = 5;
        }
      }
      break;

    case 5: 
      digitalWrite(pin_M1,     LOW);
      digitalWrite(pin_Q2,     LOW);
      digitalWrite(pin_ligada, LOW);
      Serial.println("Ciclo finalizado. Aguardando novo START.");
      estado = 0;
      break;
  }
}
