

const int pin_b1 = 2;  
const int pin_b0 = 3;  
const int pin_S1 = 4;  
const int pin_S2 = 5; 
const int pin_S3 = 6;  
const int pin_S4 = 7;  
const int pin_S5 = 8;  

const int pin_M1     = 9;
const int pin_C1     = 10;
const int pin_C2     = 11; 
const int pin_ligada = 12;

int estado = 0;
int tipo_material = 0;

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
        tipo_material = 0;
        estado = 1;
        Serial.println("Aguardando peca em S1...");
      }
      break;

    case 1: 
      if (S1 == HIGH) {
        digitalWrite(pin_M1, HIGH);
        Serial.println("Peca detectada. Esteira ligada.");
        estado = 2;
      }
      break;

    case 2:
      if (S2 == HIGH) {
        tipo_material = 1; // metal
        Serial.println("Material: METAL.");
        estado = 3;
      } else if (S3 == HIGH) {
        tipo_material = 2; // plastico
        Serial.println("Material: PLASTICO.");
        estado = 3;
      }
      break;

    case 3: 
      digitalWrite(pin_M1, LOW);
      if (tipo_material == 1) {
        digitalWrite(pin_C2, HIGH);
        Serial.println("C2 avancado: rampa METAL.");
        estado = 4;
      } else if (tipo_material == 2) {
        digitalWrite(pin_C1, HIGH);
        Serial.println("C1 avancado: rampa PLASTICO.");
        estado = 5;
      }
      break;

    case 4: 
      if (S4 == HIGH) {
        digitalWrite(pin_C2, LOW); // recua
        Serial.println("C2 recuado. Ciclo finalizado.");
        digitalWrite(pin_ligada, LOW);
        estado = 0;
      }
      break;

    case 5:
      if (S5 == HIGH) {
        digitalWrite(pin_C1, LOW); // recua
        Serial.println("C1 recuado. Ciclo finalizado.");
        digitalWrite(pin_ligada, LOW);
        estado = 0;
      }
      break;
  }
}
