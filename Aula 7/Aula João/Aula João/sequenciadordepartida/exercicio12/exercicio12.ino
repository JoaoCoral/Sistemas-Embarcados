// ============================================================
//  Sequenciador de Partida
//  - Botão 1 no pino 2  → inicia sequência (LEDs acendem 1 a 1)
//  - Botão 2 no pino 3  → apaga todos os LEDs imediatamente
//  - LED 1 no pino 5
//  - LED 2 no pino 6
//  - LED 3 no pino 7
//  - LED 4 no pino 8
// ============================================================

const int PIN_BOTAO1 = 2;
const int PIN_BOTAO2 = 3;

const int PINOS_LED[]   = {5, 6, 7, 8};
const int NUM_LEDS      = 4;
const unsigned long INTERVALO_MS = 1000; // 1 s entre cada LED
const unsigned long DEBOUNCE_MS  =   30;

// --- Máquina de estados ---
enum Estado { AGUARDANDO, SEQUENCIANDO, COMPLETO };
Estado estado = AGUARDANDO;

int           ledAtual     = 0;      // índice do próximo LED a acender
unsigned long ultimoAcende = 0;      // timestamp do último LED aceso

// ============================================================
void setup() {
  pinMode(PIN_BOTAO1, INPUT_PULLUP);
  pinMode(PIN_BOTAO2, INPUT_PULLUP);

  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(PINOS_LED[i], OUTPUT);
    digitalWrite(PINOS_LED[i], LOW);
  }

  Serial.begin(9600);
  Serial.println("Sistema pronto.");
  Serial.println("Botão 1 → inicia sequência | Botão 2 → apaga tudo");
}

// ============================================================
bool leituraBotao(int pino) {
  if (digitalRead(pino) == LOW) {
    delay(DEBOUNCE_MS);
    return digitalRead(pino) == LOW;
  }
  return false;
}

void apagarTodos() {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(PINOS_LED[i], LOW);
  }
  ledAtual = 0;
  estado   = AGUARDANDO;
  Serial.println("✖ Todos os LEDs apagados — aguardando\n");
}

void iniciarSequencia() {
  apagarTodos();               // garante estado limpo
  estado       = SEQUENCIANDO;
  ledAtual     = 0;
  ultimoAcende = millis();     // acende o LED 1 já no próximo tick

  // Acende o primeiro LED imediatamente
  digitalWrite(PINOS_LED[ledAtual], HIGH);
  Serial.print("▶ LED ");
  Serial.print(ledAtual + 1);
  Serial.println(" aceso");
  ledAtual++;
}

// ============================================================
void loop() {

  // --- Botão 2: cancela em qualquer estado ---
  if (leituraBotao(PIN_BOTAO2) && estado != AGUARDANDO) {
    apagarTodos();
    // Aguarda soltar o botão para não re-disparar
    while (digitalRead(PIN_BOTAO2) == LOW);
    return;
  }

  switch (estado) {

    // --------------------------------------------------------
    case AGUARDANDO:
      if (leituraBotao(PIN_BOTAO1)) {
        Serial.println("▼ Botão 1 pressionado — iniciando sequência");
        iniciarSequencia();
        while (digitalRead(PIN_BOTAO1) == LOW); // aguarda soltar
      }
      break;

    // --------------------------------------------------------
    case SEQUENCIANDO:
      // Botão 1 durante sequência: reinicia do zero
      if (leituraBotao(PIN_BOTAO1)) {
        Serial.println("↺ Reiniciando sequência");
        iniciarSequencia();
        while (digitalRead(PIN_BOTAO1) == LOW);
        break;
      }

      // Avança para o próximo LED após 1 segundo
      if (millis() - ultimoAcende >= INTERVALO_MS) {
        ultimoAcende = millis();

        if (ledAtual < NUM_LEDS) {
          digitalWrite(PINOS_LED[ledAtual], HIGH);
          Serial.print("▶ LED ");
          Serial.print(ledAtual + 1);
          Serial.println(" aceso");
          ledAtual++;
        }

        // Todos acesos → sequência completa
        if (ledAtual == NUM_LEDS) {
          estado = COMPLETO;
          Serial.println("✔ Sequência completa — todos os LEDs acesos\n");
        }
      }
      break;

    // --------------------------------------------------------
    case COMPLETO:
      // Apenas aguarda o Botão 2 (tratado no topo do loop)
      if (leituraBotao(PIN_BOTAO1)) {
        Serial.println("↺ Reiniciando sequência");
        iniciarSequencia();
        while (digitalRead(PIN_BOTAO1) == LOW);
      }
      break;
  }
}
