

const int pin_b1 = 2;  
const int pin_b0 = 3;  
const int pin_S1 = 4;  
const int pin_S2 = 5;  
const int pin_S3 = 6;  
const int pin_S4 = 7;  

const int pin_M1     = 8;
const int pin_R1     = 9;  
const int pin_V1     = 10; 
const int pin_ligada = 11;

int estado = 0;
unsigned long tempo_ant  = 0;
unsigned long tempo_atual = 0;

void desligar_tudo() {
  digitalWrite(pin_M1,     LOW);
  digitalWrite(pin_R1,     LOW);
  digitalWrite(pin_V1,     LOW);
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
  pinMode(pin_R1,     OUTPUT);
  pinMode(pin_V1,     OUTPUT);
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
        digitalWrite(pin_ligada, HIGH);
        Serial.println("Sistema iniciado. Aguardando peca em S1.");
        estado = 1;
      }
      break;

    case 1: 
      if (S1 == HIGH) {
        digitalWrite(pin_M1, HIGH);
        Serial.println("Peca detectada em S1. Esteira ligada.");
        estado = 2;
      }
      break;

    case 2: 
      if (S2 == HIGH) {
        digitalWrite(pin_M1, LOW);
        digitalWrite(pin_R1, HIGH);
        tempo_ant = tempo_atual;
        Serial.println("Peca em S2. Aquecendo (3s).");
        estado = 3;
      }
      break;

    case 3: 
      if (tempo_atual - tempo_ant >= 3000) {
        digitalWrite(pin_R1, LOW);
        digitalWrite(pin_M1, HIGH);
        Serial.println("Aquecimento concluido. Transportando para S3.");
        estado = 4;
      }
      break;

    case 4: 
      if (S3 == HIGH) {
        digitalWrite(pin_M1, LOW);
        digitalWrite(pin_V1, HIGH);
        tempo_ant = tempo_atual;
        Serial.println("Peca em S3. Resfriando (2s).");
        estado = 5;
      }
      break;

    case 5: 
      if (tempo_atual - tempo_ant >= 2000) {
        digitalWrite(pin_V1, LOW);
        digitalWrite(pin_M1, HIGH);
        Serial.println("Resfriamento concluido. Transportando para saida.");
        estado = 6;
      }
      break;

    case 6: 
      if (S4 == HIGH) {
        digitalWrite(pin_M1,     LOW);
        digitalWrite(pin_ligada, LOW);
        Serial.println("Peca na caixa. Ciclo finalizado.");
        estado = 0;
      }
      break;
  }
}
