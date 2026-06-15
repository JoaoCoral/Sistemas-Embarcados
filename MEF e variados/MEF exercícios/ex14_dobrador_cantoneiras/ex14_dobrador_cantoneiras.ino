
const int pin_b1 = 2; 
const int pin_b0 = 3; 
const int pin_S4 = 4;  
const int pin_S1 = 5; 
const int pin_S2 = 6; 
const int pin_S3 = 7; 

const int pin_M1     = 8;
const int pin_C1     = 9;  
const int pin_C2     = 10; 
const int pin_C3     = 11; 
const int pin_ligada = 12;

int estado = 0;
unsigned long tempo_ant  = 0;
unsigned long tempo_atual = 0;

void desligar_tudo() {
  digitalWrite(pin_M1,     LOW);
  digitalWrite(pin_C1,     LOW);
  digitalWrite(pin_C2,     LOW);
  digitalWrite(pin_C3,     LOW);
  digitalWrite(pin_ligada, LOW);
}

void setup() {
  pinMode(pin_b1, INPUT_PULLUP);
  pinMode(pin_b0, INPUT_PULLUP);
  pinMode(pin_S4, INPUT);
  pinMode(pin_S1, INPUT);
  pinMode(pin_S2, INPUT);
  pinMode(pin_S3, INPUT);

  pinMode(pin_M1,     OUTPUT);
  pinMode(pin_C1,     OUTPUT);
  pinMode(pin_C2,     OUTPUT);
  pinMode(pin_C3,     OUTPUT);
  pinMode(pin_ligada, OUTPUT);

  Serial.begin(9600);
  desligar_tudo();
}

void loop() {
  tempo_atual = millis();

  bool start = (digitalRead(pin_b1) == LOW);
  bool stop  = (digitalRead(pin_b0) == LOW);
  bool S4    = digitalRead(pin_S4);
  bool S1    = digitalRead(pin_S1);
  bool S2    = digitalRead(pin_S2);
  bool S3    = digitalRead(pin_S3);

  if (stop) {
    desligar_tudo();
    estado = 0;
    return;
  }

  switch (estado) {

    case 0: 
      desligar_tudo();
      if (start) {
        digitalWrite(pin_M1,     HIGH);
        digitalWrite(pin_ligada, HIGH);
        Serial.println("M1 ligado. Aguardando material em S4.");
        estado = 1;
      }
      break;

    case 1: 
      if (S4 == HIGH) {
        digitalWrite(pin_M1, LOW); 
        Serial.println("Material em S4. C1 avancando (fixador).");
        digitalWrite(pin_C1, HIGH);
        estado = 2;
      }
      break;

    case 2: 
      if (S1 == HIGH) {
        tempo_ant = tempo_atual;
        Serial.println("C1 avancado. Aguardando 2s para C3 (dobra).");
        estado = 3;
      }
      break;

    case 3: 
      if (tempo_atual - tempo_ant >= 2000) {
        digitalWrite(pin_C3, HIGH);
        Serial.println("C3 avancando (dobra).");
        estado = 4;
      }
      break;

    case 4: 
      if (S3 == HIGH) {
        tempo_ant = tempo_atual;
        Serial.println("C3 avancado. Aguardando 2s para C2 (corte).");
        estado = 5;
      }
      break;

    case 5:
      if (tempo_atual - tempo_ant >= 2000) {
        digitalWrite(pin_C2, HIGH);
        Serial.println("C2 avancando (corte).");
        estado = 6;
      }
      break;

    case 6: 
      if (S2 == HIGH) {
        digitalWrite(pin_C1, LOW);
        digitalWrite(pin_C2, LOW);
        digitalWrite(pin_C3, LOW);
        Serial.println("Corte concluido. Todos cilindros recuando.");
        estado = 7;
      }
      break;

    case 7:
      digitalWrite(pin_ligada, LOW);
      Serial.println("Ciclo finalizado.");
      estado = 0;
      break;
  }
}
