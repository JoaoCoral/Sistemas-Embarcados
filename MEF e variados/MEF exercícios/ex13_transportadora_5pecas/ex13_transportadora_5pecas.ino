

const int pin_b1 = 2;  
const int pin_b0 = 3;  
const int pin_S1 = 4;  
const int pin_S2 = 5;  
const int pin_S3 = 6;  

const int pin_M1     = 7;
const int pin_M2     = 8;
const int pin_ligada = 9;
const int pin_limite = 10; 

int estado      = 0;
int cont_pecas  = 0;

void desligar_tudo() {
  digitalWrite(pin_M1,     LOW);
  digitalWrite(pin_M2,     LOW);
  digitalWrite(pin_ligada, LOW);
}

void setup() {
  pinMode(pin_b1, INPUT_PULLUP);
  pinMode(pin_b0, INPUT_PULLUP);
  pinMode(pin_S1, INPUT);
  pinMode(pin_S2, INPUT);
  pinMode(pin_S3, INPUT);

  pinMode(pin_M1,     OUTPUT);
  pinMode(pin_M2,     OUTPUT);
  pinMode(pin_ligada, OUTPUT);
  pinMode(pin_limite, OUTPUT);

  Serial.begin(9600);
  desligar_tudo();
  digitalWrite(pin_limite, LOW);
}

void loop() {
  bool start = (digitalRead(pin_b1) == LOW);
  bool stop  = (digitalRead(pin_b0) == LOW);
  bool S1    = digitalRead(pin_S1);
  bool S2    = digitalRead(pin_S2);
  bool S3    = digitalRead(pin_S3);

  if (stop) {
    desligar_tudo();
    digitalWrite(pin_limite, LOW);
    cont_pecas = 0;
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
        Serial.println("Peca detectada. M1 ligado.");
        estado = 2;
      }
      break;

    case 2: 
      if (S2 == HIGH) {
        digitalWrite(pin_M1, LOW);
        digitalWrite(pin_M2, HIGH); 
        Serial.println("Peca em S2. M1 desligado, M2 ligado.");
        estado = 3;
      }
      break;

    case 3: 
      if (S3 == HIGH) {
        digitalWrite(pin_M2, LOW);
        cont_pecas++;
        Serial.print("Pecas transportadas: ");
        Serial.println(cont_pecas);
        if (cont_pecas >= 5) {
          digitalWrite(pin_limite, HIGH); 
          Serial.println("LIMITE de 5 pecas atingido! Pressione STOP.");
          estado = 4; // aguarda STOP
        } else {
          estado = 1; 
        }
      }
      break;

    case 4:
      break;
  }
}
