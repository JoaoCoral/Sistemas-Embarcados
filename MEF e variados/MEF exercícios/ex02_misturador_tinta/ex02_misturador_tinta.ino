

const int pin_start1     = 2; 
const int pin_stop1      = 3; 
const int pin_start2     = 4; 
const int pin_stop2      = 5; 
const int pin_high_level = 6;
const int pin_low_level  = 7;

const int pin_pump1      = 8;
const int pin_pump2      = 9;
const int pin_mixer      = 10;
const int pin_steam      = 11;
const int pin_drain_v    = 12;
const int pin_drain_p    = 13;

int estado        = 0;
int ciclos        = 0;
unsigned long tempo_ant  = 0;
unsigned long tempo_atual = 0;

void desligar_tudo() {
  digitalWrite(pin_pump1,   LOW);
  digitalWrite(pin_pump2,   LOW);
  digitalWrite(pin_mixer,   LOW);
  digitalWrite(pin_steam,   LOW);
  digitalWrite(pin_drain_v, LOW);
  digitalWrite(pin_drain_p, LOW);
}

void setup() {
  pinMode(pin_start1,     INPUT_PULLUP);
  pinMode(pin_stop1,      INPUT_PULLUP);
  pinMode(pin_start2,     INPUT_PULLUP);
  pinMode(pin_stop2,      INPUT_PULLUP);
  pinMode(pin_high_level, INPUT);
  pinMode(pin_low_level,  INPUT);

  pinMode(pin_pump1,   OUTPUT);
  pinMode(pin_pump2,   OUTPUT);
  pinMode(pin_mixer,   OUTPUT);
  pinMode(pin_steam,   OUTPUT);
  pinMode(pin_drain_v, OUTPUT);
  pinMode(pin_drain_p, OUTPUT);

  Serial.begin(9600);
  desligar_tudo();
}

void loop() {
  tempo_atual = millis();

  bool start1     = (digitalRead(pin_start1) == LOW);
  bool stop1      = (digitalRead(pin_stop1)  == LOW); 
  bool start2     = (digitalRead(pin_start2) == LOW);
  bool stop2      = (digitalRead(pin_stop2)  == LOW);
  bool high_level = digitalRead(pin_high_level);
  bool low_level  = digitalRead(pin_low_level);

  if (stop1 || stop2) {
    desligar_tudo();
    estado = 0;
  }

  switch (estado) {

    case 0: 
      desligar_tudo();
      if (start1 || start2) {
        estado = 1;
        Serial.println("Ciclo iniciado.");
      }
      break;

    case 1: 
      digitalWrite(pin_pump1, HIGH);
      Serial.println("Passo 1: enchendo ingrediente 1...");
      if (start2) {
        digitalWrite(pin_pump2, HIGH);
        Serial.println("Passo 2: enchendo ingrediente 2 simultaneamente...");
      }
      estado = 2;
      break;

    case 2:
      if (high_level == HIGH) {
        digitalWrite(pin_pump1, LOW);
        digitalWrite(pin_pump2, LOW);
        Serial.println("Passo 3: nivel alto atingido.");
        estado = 3;
      }
      break;

    case 3:
      if (!stop1 && !stop2) {
        estado = 4;
      }
      break;

    case 4: 
      digitalWrite(pin_mixer, HIGH);
      digitalWrite(pin_steam, HIGH);
      Serial.println("Passo 5/6: misturando e aquecendo (10s)...");
      tempo_ant = tempo_atual;
      estado = 5;
      break;

    case 5: 
      if (tempo_atual - tempo_ant >= 10000) {
        digitalWrite(pin_mixer, LOW);
        digitalWrite(pin_steam, LOW);
        Serial.println("Mistura concluida.");
        estado = 6;
      }
      break;

    case 6: 
      digitalWrite(pin_drain_v, HIGH);
      digitalWrite(pin_drain_p, HIGH);
      Serial.println("Passo 7: drenando tanque...");
      estado = 7;
      break;

    case 7: 
      if (low_level == LOW) { 
        digitalWrite(pin_drain_v, LOW);
        digitalWrite(pin_drain_p, LOW);
        ciclos++;
        Serial.print("Passo 8: Ciclo completo! Total de ciclos: ");
        Serial.println(ciclos);
        estado = 0; 
      }
      break;
  }
}
