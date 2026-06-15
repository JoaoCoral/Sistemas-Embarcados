

const int pin_start = 2;  
const int pin_stop  = 3;  
const int pin_FT    = 4;  
const int pin_SP1   = 5;  
const int pin_ST1   = 6;  
const int pin_ST2   = 7;  
const int pin_SR    = 8; 
const int pin_SA    = 9;  

const int pin_M1    = 10; 
const int pin_CR    = 11; 
const int pin_FC    = 12; 

int estado         = 0;
int cont_garrafas  = 0;
unsigned long tempo_ant  = 0;
unsigned long tempo_atual = 0;

void desligar_tudo() {
  digitalWrite(pin_M1, LOW);
  digitalWrite(pin_CR, LOW);
  digitalWrite(pin_FC, LOW);
}

void setup() {
  pinMode(pin_start, INPUT_PULLUP);
  pinMode(pin_stop,  INPUT_PULLUP);
  pinMode(pin_FT,    INPUT);
  pinMode(pin_SP1,   INPUT);
  pinMode(pin_ST1,   INPUT);
  pinMode(pin_ST2,   INPUT);
  pinMode(pin_SR,    INPUT);
  pinMode(pin_SA,    INPUT);

  pinMode(pin_M1, OUTPUT);
  pinMode(pin_CR, OUTPUT);
  pinMode(pin_FC, OUTPUT);

  Serial.begin(9600);
  desligar_tudo();
}

void loop() {
  tempo_atual = millis();

  bool btn_start = (digitalRead(pin_start) == LOW);
  bool btn_stop  = (digitalRead(pin_stop)  == LOW);
  bool FT        = digitalRead(pin_FT);
  bool SP1       = digitalRead(pin_SP1);
  bool ST1       = digitalRead(pin_ST1);
  bool ST2       = digitalRead(pin_ST2);
  bool SR        = digitalRead(pin_SR);
  bool SA        = digitalRead(pin_SA);

  
  if (btn_stop) {
    desligar_tudo();
    estado = 0;
    return;
  }

 
  if (FT && (estado == 1)) {
    digitalWrite(pin_M1, LOW);
    estado = 8; 
  }

  switch (estado) {

    case 0: 
      desligar_tudo();
      if (btn_start) {
        if (cont_garrafas >= 24) {
          cont_garrafas = 0;
          Serial.println("Contador zerado. Novo ciclo de 24 garrafas.");
        }
        estado = 1;
        Serial.println("Sistema iniciado.");
      }
      break;

    case 1: 
      digitalWrite(pin_M1, HIGH);
      if (SP1 == HIGH) { 
        digitalWrite(pin_M1, LOW);
        estado = 2;
      }
      break;

    case 2: 
      if (ST2 == LOW) { 
        estado = 3; 
      }
      if (SR == HIGH) {
        digitalWrite(pin_CR, HIGH);
      }
      estado = 3;
      break;

    case 3: 
      digitalWrite(pin_CR, HIGH);
      if (SA == HIGH) {
        tempo_ant = tempo_atual;
        estado = 4;
      }
      break;

    case 4: 
      if (tempo_atual - tempo_ant >= 2000) {
        digitalWrite(pin_CR, LOW); // recua
        estado = 5;
      }
      break;

    case 5: 
      if (SR == HIGH) {
        if (ST2 == LOW) {
          digitalWrite(pin_FC, HIGH);
          Serial.println("ALARME: garrafa sem tampa em P2!");
          estado = 9; 
        } else {
          cont_garrafas++;
          Serial.print("Garrafas tampadas: ");
          Serial.println(cont_garrafas);
          if (cont_garrafas >= 24) {
            Serial.println("Lote de 24 garrafas concluido! Pressione STOP.");
            estado = 7; 
          } else {
            estado = 1; 
          }
        }
      }
      break;

    case 7:
      break;

    case 8:
      digitalWrite(pin_M1, LOW);
      Serial.println("ALARME: Falta de tampinhas!");
      break;

    case 9: 
      digitalWrite(pin_FC, HIGH);
      if (btn_start) {
        digitalWrite(pin_FC, LOW);
        estado = 1;
      }
      break;
  }
}
