
const int pin_b1 = 2;  
const int pin_b0 = 3;  
const int pin_S1 = 4;  
const int pin_S2 = 5;  
const int pin_S3 = 6;  
const int pin_S4 = 7;  

const int pin_M1     = 8;
const int pin_M2     = 9;
const int pin_C1     = 10;
const int pin_C2     = 11;
const int pin_ligada = 12;

int estado = 0;

void desligar_tudo() {
  digitalWrite(pin_M1,     LOW);
  digitalWrite(pin_M2,     LOW);
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

  pinMode(pin_M1,     OUTPUT);
  pinMode(pin_M2,     OUTPUT);
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
      if (S2 == HIGH) {
        digitalWrite(pin_M1, LOW);
        Serial.println("Peca em posicao. Avancando C1.");
        estado = 2;
      }
      break;

    case 2: 
      digitalWrite(pin_C1, HIGH);
      if (S1 == HIGH) {
        Serial.println("C1 avancado. Avancando C2.");
        estado = 3;
      }
      break;

    case 3: 
      digitalWrite(pin_C2, HIGH);
      if (S3 == HIGH) {
        Serial.println("C2 avancado. Ligando M2.");
        estado = 4;
      }
      break;

    case 4: 
      digitalWrite(pin_M2, HIGH);
      if (S4 == HIGH) {
        digitalWrite(pin_M2, LOW);
        Serial.println("Peca passou S4. Recuando cilindros.");
        estado = 5;
      }
      break;

    case 5: 
      digitalWrite(pin_C1, LOW);
      digitalWrite(pin_C2, LOW);
      Serial.println("Ciclo finalizado.");
      digitalWrite(pin_ligada, LOW);
      estado = 0; 
      break;
  }
}
