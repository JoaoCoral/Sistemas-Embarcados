

const int pin_b1 = 2;  
const int pin_b0 = 3;  
const int pin_S1 = 4;  
const int pin_S2 = 5;  
const int pin_S3 = 6;  

const int pin_M1     = 7;
const int pin_M2     = 8;
const int pin_C1     = 9;
const int pin_ligada = 10;

int estado       = 0;
int cont_cortes  = 0;
unsigned long tempo_ant  = 0;
unsigned long tempo_atual = 0;

bool M1_pronto = false;
bool M2_pronto = false;

void desligar_tudo() {
  digitalWrite(pin_M1,     LOW);
  digitalWrite(pin_M2,     LOW);
  digitalWrite(pin_C1,     LOW);
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
  pinMode(pin_C1,     OUTPUT);
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

  if (stop) {
    desligar_tudo();
    estado = 0;
    cont_cortes = 0;
    return;
  }

  switch (estado) {

    case 0: 
      desligar_tudo();
      if (start) {
        M1_pronto  = false;
        M2_pronto  = false;
        cont_cortes = 0;
        digitalWrite(pin_ligada, HIGH);
        estado = 1;
        Serial.println("Ciclo iniciado.");
      }
      break;

    case 1: 
      digitalWrite(pin_M1, HIGH);
      digitalWrite(pin_M2, HIGH);
      Serial.println("M1 e M2 ligados simultaneamente.");
      estado = 2;
      break;

    case 2: 
      if (S2 == HIGH) {
        digitalWrite(pin_M1, LOW);
        M1_pronto = true;
      }
      if (S3 == HIGH) {
        digitalWrite(pin_M2, LOW);
        M2_pronto = true;
      }
      if (M1_pronto && M2_pronto) {
        M1_pronto = false;
        M2_pronto = false;
        estado = 3;
      }
      break;

    case 3: 
      digitalWrite(pin_C1, HIGH);
      Serial.print("Corte ");
      Serial.print(cont_cortes + 1);
      Serial.println(" iniciado.");
      if (S1 == HIGH) { 
        tempo_ant = tempo_atual;
        estado = 4;
      }
      break;

    case 4: 
      digitalWrite(pin_C1, LOW);
      if (tempo_atual - tempo_ant >= 500) {
        cont_cortes++;
        Serial.print("Cortes realizados: ");
        Serial.println(cont_cortes);
        if (cont_cortes >= 5) {
          cont_cortes = 0;
          estado = 5; 
        } else {
          estado = 3; 
        }
      }
      break;

    case 5: 
      digitalWrite(pin_M2, HIGH);
      Serial.println("5 cortes. Avancando caixa...");
      estado = 6;
      break;

    case 6: 
      if (S3 == HIGH) {
        digitalWrite(pin_M2, LOW);
        Serial.println("Nova caixa posicionada. Pronto para novo ciclo.");
        // reinicia ciclo de chapas
        M1_pronto = false;
        M2_pronto = false;
        estado = 7; 
      }
      break;

    case 7:
      digitalWrite(pin_M1, HIGH);
      estado = 8;
      break;

    case 8:
      if (S2 == HIGH) {
        digitalWrite(pin_M1, LOW);
        estado = 3; 
      }
      break;
  }
}
