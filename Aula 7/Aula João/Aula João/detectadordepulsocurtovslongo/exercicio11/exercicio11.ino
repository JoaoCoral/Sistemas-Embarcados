// ============================================================
//  Detector de Pulso Curto vs. Longo
//  - Botão A no pino 2
//  - LED 1 no pino 9  → pressionamento curto  (< 1 s)
//  - LED 2 no pino 6  → pressionamento longo   (> 2 s)
// ============================================================

const int PIN_BOTAO_A = 2;
const int PIN_LED1    = 9;
const int PIN_LED2    = 6;

const unsigned long TEMPO_CURTO  = 1000; // < 1 s  → LED 1
const unsigned long TEMPO_LONGO  = 2000; // > 2 s  → LED 2
const unsigned long TEMPO_LED_ON = 3000; // LEDs ficam acesos por 3 s
const unsigned long DEBOUNCE_MS  =   30; // filtro de ruído mecânico

// --- Máquina de estados ---
enum Estado { AGUARDANDO, PRESSIONADO, EXIBINDO };
Estado estado = AGUARDANDO;

unsigned long tempoPress    = 0; // momento em que o botão foi pressionado
unsigned long tempoLedInicio = 0; // momento em que o LED acendeu
int           ledAtivo      = 0; // qual LED está aceso (1 ou 2)

// ============================================================
void setup() {
  pinMode(PIN_BOTAO_A, INPUT_PULLUP);
  pinMode(PIN_LED1,    OUTPUT);
  pinMode(PIN_LED2,    OUTPUT);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);
  Serial.begin(9600);
  Serial.println("Sistema pronto — pressione o Botão A");
}

// ============================================================
// Lê o botão com debounce por confirmação dupla
bool botaoPresionado() {
  if (digitalRead(PIN_BOTAO_A) == LOW) {
    delay(DEBOUNCE_MS);
    return digitalRead(PIN_BOTAO_A) == LOW;
  }
  return false;
}

bool botaoSolto() {
  if (digitalRead(PIN_BOTAO_A) == HIGH) {
    delay(DEBOUNCE_MS);
    return digitalRead(PIN_BOTAO_A) == HIGH;
  }
  return false;
}

// ============================================================
void apagarLeds() {
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);
  ledAtivo = 0;
}

void acenderLed(int led) {
  apagarLeds();
  digitalWrite(led == 1 ? PIN_LED1 : PIN_LED2, HIGH);
  ledAtivo      = led;
  tempoLedInicio = millis();

  Serial.print("LED ");
  Serial.print(led);
  Serial.println(led == 1 ? " aceso (pulso CURTO < 1 s)" : " aceso (pulso LONGO > 2 s)");
}

// ============================================================
void loop() {

  switch (estado) {

    // --------------------------------------------------------
    case AGUARDANDO:
      if (botaoPresionado()) {
        tempoPress = millis();
        estado     = PRESSIONADO;
        Serial.println("Botão pressionado...");
      }
      break;

    // --------------------------------------------------------
    case PRESSIONADO:
      // Feedback visual enquanto segura: pisca o LED 2 após 2 s
      if (millis() - tempoPress >= TEMPO_LONGO) {
        digitalWrite(PIN_LED2, (millis() / 200) % 2); // pisca a 5 Hz
      }

      if (botaoSolto()) {
        unsigned long duracao = millis() - tempoPress;
        digitalWrite(PIN_LED2, LOW); // garante que o pisca pare

        Serial.print(" Botão solto — duração: ");
        Serial.print(duracao);
        Serial.println(" ms");

        if (duracao < TEMPO_CURTO) {
          acenderLed(1);
          estado = EXIBINDO;

        } else if (duracao >= TEMPO_LONGO) {
          acenderLed(2);
          estado = EXIBINDO;

        } else {
          // Entre 1 s e 2 s: zona neutra, nenhum LED acende
          Serial.println("○ Zona neutra (1 s – 2 s) — nenhum LED aceso");
          estado = AGUARDANDO;
        }
      }
      break;

    // --------------------------------------------------------
    case EXIBINDO:
      // Apaga o LED após 3 segundos
      if (millis() - tempoLedInicio >= TEMPO_LED_ON) {
        apagarLeds();
        estado = AGUARDANDO;
        Serial.println("○ LED apagado — aguardando novo pulso\n");
      }
      break;
  }
}
