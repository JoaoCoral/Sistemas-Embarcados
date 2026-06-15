

const int pin_am    = 2;
const int pin_liga  = 3;  
const int pin_desl  = 4;  
const int pin_n_baixo = 5; 
const int pin_n_alto  = 6; 
const int pin_bomba = 7;

int estado = 0;
unsigned long tempo_ant  = 0;
unsigned long tempo_atual = 0;

void setup() {
  pinMode(pin_am,      INPUT_PULLUP);
  pinMode(pin_liga,    INPUT_PULLUP);
  pinMode(pin_desl,    INPUT_PULLUP);
  pinMode(pin_n_baixo, INPUT);
  pinMode(pin_n_alto,  INPUT);
  pinMode(pin_bomba,   OUTPUT);
  Serial.begin(9600);
}

void loop() {
  tempo_atual = millis();

  bool modo_auto  = (digitalRead(pin_am) == LOW); 
  bool btn_liga   = (digitalRead(pin_liga) == LOW);
  bool btn_desl   = (digitalRead(pin_desl) == LOW);
  bool n_baixo    = digitalRead(pin_n_baixo);
  bool n_alto     = digitalRead(pin_n_alto);


  if (!modo_auto && btn_desl) {
    estado = 0;
    digitalWrite(pin_bomba, LOW);
  }

  switch (estado) {

    case 0:
      digitalWrite(pin_bomba, LOW);
      if (modo_auto) {
        estado = 2; 
      } else {
        if (btn_liga) {
          estado = 1;
        }
      }
      break;

    case 1:
      digitalWrite(pin_bomba, HIGH);
      Serial.println("MANUAL: Bomba LIGADA");
      if (!modo_auto) {
      }
      break;

    case 2:
      digitalWrite(pin_bomba, LOW);
      Serial.println("AUTO: aguardando nivel baixo...");
      if (!modo_auto) { estado = 0; break; }
      if (n_baixo == HIGH) {
        tempo_ant = tempo_atual;
        estado = 3;
      }
      break;

    case 3: 
      if (!modo_auto) { estado = 0; break; }
      if (tempo_atual - tempo_ant >= 10000) {
        estado = 4;
      }
      break;

    case 4: 
      digitalWrite(pin_bomba, HIGH);
      Serial.println("AUTO: Bomba LIGADA (nivel baixo)");
      if (!modo_auto) { estado = 0; break; }
      if (n_alto == HIGH) { 
        tempo_ant = tempo_atual;
        estado = 5;
      }
      break;

    case 5: 
      if (!modo_auto) { estado = 0; break; }
      if (tempo_atual - tempo_ant >= 10000) {
        digitalWrite(pin_bomba, LOW);
        Serial.println("AUTO: Bomba DESLIGADA (nivel alto)");
        estado = 2;
      }
      break;
  }
}
