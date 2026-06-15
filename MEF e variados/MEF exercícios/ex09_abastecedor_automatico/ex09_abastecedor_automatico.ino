
const int pin_b1 = 2;  
const int pin_b0 = 3;  
const int pin_S1 = 4;  
const int pin_S2 = 5;  
const int pin_S3 = 6;  
const int pin_S4 = 7;  

const int pin_M1     = 8;
const int pin_C1     = 9;
const int pin_C2     = 10;
const int pin_ligada = 11;

int estado = 0;
unsigned long tempo_ant  = 0;
unsigned long tempo_atual = 0;

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

  pinMode(pin_M1,     OUTPUT);
  pinMode(pin_C1,     OUTPUT);
  pinMode(pin_C2,     OUTPUT);
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
        digitalWrite(pin_M1,     HIGH);
        digitalWrite(pin_C1,     HIGH); 
        digitalWrite(pin_ligada, HIGH);
        Serial.println("Esteira e C1 ligados.");
        estado = 1;
      }
      break;

    case 1: 
      if (S1 == HIGH) {
        digitalWrite(pin_C1, LOW); 
        Serial.println("Caixa passou S1. C1 recuando.");
        estado = 2;
      }
      break;

    case 2: 
      if (S2 == HIGH) {
        digitalWrite(pin_M1, LOW); 
        Serial.println("Caixa em S2. Iniciando enchimento (C2 recua).");
        digitalWrite(pin_C2, LOW); 
        estado = 3;
      }
      break;

    case 3: 
      if (S3 == HIGH) {
        tempo_ant = tempo_atual;
        Serial.println("Enchendo caixa por 6 segundos...");
        estado = 4;
      }
      break;

    case 4: 
      if (tempo_atual - tempo_ant >= 6000) {
        digitalWrite(pin_C2, HIGH); 
        Serial.println("Enchimento concluido. C2 avancando.");
        estado = 5;
      }
      break;

    case 5:
      if (digitalRead(pin_S3) == LOW) { 
        digitalWrite(pin_M1, HIGH);
        Serial.println("Esteira ligada. Transportando para saida.");
        estado = 6;
      }
      break;

    case 6:
      if (S4 == HIGH) {
        digitalWrite(pin_M1, LOW);
        Serial.println("Caixa em S4. Aguardando operador retirar.");
        estado = 7;
      }
      break;

    case 7: 
      if (S4 == LOW) {
        Serial.println("Caixa retirada. Ciclo finalizado.");
        digitalWrite(pin_ligada, LOW);
        estado = 0;
      }
      break;
  }
}
