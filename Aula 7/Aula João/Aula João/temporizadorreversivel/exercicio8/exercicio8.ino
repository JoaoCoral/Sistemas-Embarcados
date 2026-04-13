// ============================================================
//  Temporizador Reversível
//  - Botão A no pino 2  → inicia temporizador de 5s (LED 1 acende)
//  - Botão B no pino 3  → cancela imediatamente (LED 1 apaga)
//  - LED 1 no pino 6
// ============================================================

const int PIN_BOTAO_A = 2;
const int PIN_BOTAO_B = 3;
const int PIN_LED1    = 6;

const unsigned long TEMPO_MS = 5000; // 5 segundos

bool     timerAtivo  = false;
unsigned long inicioTimer = 0;

// ============================================================
void setup() {
  pinMode(PIN_BOTAO_A, INPUT_PULLUP);
  pinMode(PIN_BOTAO_B, INPUT_PULLUP);
  pinMode(PIN_LED1,    OUTPUT);
  digitalWrite(PIN_LED1, LOW);
  Serial.begin(9600);
  Serial.println("Sistema pronto. Pressione o Botão A para iniciar.");
}

// ============================================================
void loop() {
  bool botaoA = (digitalRead(PIN_BOTAO_A) == LOW); // LOW = pressionado (PULLUP)
  bool botaoB = (digitalRead(PIN_BOTAO_B) == LOW);

  // --- Botão A: inicia o temporizador (só se não estiver ativo) ---
  if (botaoA && !timerAtivo) {
    timerAtivo  = true;
    inicioTimer = millis();
    digitalWrite(PIN_LED1, HIGH);
    Serial.println("▶ Temporizador iniciado — LED 1 aceso por 5 s");
  }

  if (timerAtivo) {
    // --- Botão B: cancela o temporizador ---
    if (botaoB) {
      timerAtivo = false;
      digitalWrite(PIN_LED1, LOW);
      Serial.println("✖ Cancelado pelo Botão B — LED 1 apagado");
      delay(200); // debounce mínimo
      return;
    }

    // --- Tempo esgotado: apaga o LED naturalmente ---
    if (millis() - inicioTimer >= TEMPO_MS) {
      timerAtivo = false;
      digitalWrite(PIN_LED1, LOW);
      Serial.println("✔ Tempo esgotado — LED 1 apagado");
    }
  }
}
