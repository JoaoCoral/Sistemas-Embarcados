

const int pin_b1 = 2;  
const int pin_S1 = 3;  
const int pin_S2 = 4;  
const int pin_S3 = 5;  

const int pin_C1     = 6;
const int pin_C2     = 7;
const int pin_M1     = 8;  
const int pin_ligada = 9;

int estado = 0;

void desligar_tudo() {
  digitalWrite(pin_C1,     LOW);
  digitalWrite(pin_C2,     LOW);
  digitalWrite(pin_M1,     LOW);
  digitalWrite(pin_ligada, LOW);
}

void setup() {
  pinMode(pin_b1, INPUT_PULLUP);
  pinMode(pin_S1, INPUT);
  pinMode(pin_S2, INPUT);
  pinMode(pin_S3, INPUT);

  pinMode(pin_C1,     OUTPUT);
  pinMode(pin_C2,     OUTPUT);
  pinMode(pin_M1,     OUTPUT);
  pinMode(pin_ligada, OUTPUT);

  Serial.begin(9600);
  desligar_tudo();
}

void loop() {
  bool start = (digitalRead(pin_b1) == LOW);
  bool S1    = digitalRead(pin_S1);
  bool S2    = digitalRead(pin_S2);
  bool S3    = digitalRead(pin_S3);

  switch (estado) {

    case 0: 
      desligar_tudo();
      if (start) {
        digitalWrite(pin_ligada, HIGH);
        digitalWrite(pin_C1,     HIGH); 
        Serial.println("C1 avancando para prender peca.");
        estado = 1;
      }
      break;

    case 1: 
      if (S1 == HIGH) {
        digitalWrite(pin_M1, HIGH); 
        Serial.println("Peca presa. Motor da furadeira ligado.");
        estado = 2;
      }
      break;

    case 2: 
      digitalWrite(pin_C2, HIGH);
      Serial.println("C2 avancando (furando).");
      estado = 3;
      break;

    case 3: 
      if (S2 == HIGH) {
        digitalWrite(pin_C2, LOW); 
        Serial.println("Furo concluido. C2 recuando.");
        estado = 4;
      }
      break;

    case 4: 
      if (S3 == HIGH) {
        digitalWrite(pin_M1, LOW); 
        Serial.println("C2 recuado. Motor desligado. Recuando C1.");
        estado = 5;
      }
      break;

    case 5: 
      digitalWrite(pin_C1, LOW);
      Serial.println("C1 recuado. Ciclo finalizado.");
      digitalWrite(pin_ligada, LOW);
      estado = 0;
      break;
  }
}
