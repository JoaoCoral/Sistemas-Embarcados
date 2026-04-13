// ============================================================
//  Prensa Hidráulica com Comando Bi-manual (NR-12)
//  - Botão A no pino 2  → mão esquerda
//  - Botão B no pino 3  → mão direita
//  - LED 1 no pino 5    → prensa descendo (operação normal)
//  - LED 4 no pino 8    → erro de segurança
//
//  Regra: A e B pressionados com Δt < 500 ms → prensa ativa
//         Apenas um botão OU Δt ≥ 500 ms     → erro
//         Qualquer botão solto               → prensa para
// ============================================================

const int PIN_BOTAO_A = 2;
const int PIN_BOTAO_B = 3;
const int PIN_PRENSA  = 5;   // LED 1
const int PIN_ERRO    = 8;   // LED 4

const unsigned long JANELA_MS      = 500;  // Δt máximo entre os dois botões
const unsigned long TEMPO_ERRO_MS  = 2000; // LED de erro fica aceso por 2 s
const unsigned long DEBOUNCE_MS    = 25;   // filtro de ruído mecânico

// --- Máquina de estados ---
enum Estado {
  AGUARDANDO,     // nenhum botão pressionado
  PRIMEIRO_BOTAO, // um botão foi pressionado, aguardando o segundo
  OPERANDO,       // ambos dentro da janela → prensa ativa
  ERRO,           // falha de segurança → LED erro aceso
  LIBERANDO       // aguarda ambos os botões serem soltos antes de resetar
};

Estado estado = AGUARDANDO;

unsigned long tempoprimeiro   = 0;   // timestamp do 1º botão pressionado
unsigned long tempoInicioErro = 0;   // timestamp de quando o erro foi acionado
int           primeiroBotao   = 0;   // qual botão foi pressionado primeiro (A=2 / B=3)

// ============================================================
void setup() {
  pinMode(PIN_BOTAO_A, INPUT_PULLUP);
  pinMode(PIN_BOTAO_B, INPUT_PULLUP);
  pinMode(PIN_PRENSA,  OUTPUT);
  pinMode(PIN_ERRO,    OUTPUT);
  digitalWrite(PIN_PRENSA, LOW);
  digitalWrite(PIN_ERRO,   LOW);

  Serial.begin(9600);
  Serial.println("╔══════════════════════════════════════════╗");
  Serial.println("║   Prensa Bi-manual NR-12 — v1.0          ║");
  Serial.println("╠══════════════════════════════════════════╣");
  Serial.println("║  Pressione A e B em até 500 ms           ║");
  Serial.println("║  para acionar a prensa com segurança     ║");
  Serial.println("╚══════════════════════════════════════════╝\n");
}

// ============================================================
bool leituraDebounce(int pino) {
  if (digitalRead(pino) == LOW) {
    delay(DEBOUNCE_MS);
    return digitalRead(pino) == LOW;
  }
  return false;
}

bool botaoA() { return leituraDebounce(PIN_BOTAO_A); }
bool botaoB() { return leituraDebounce(PIN_BOTAO_B); }
bool ambosPressos() {
  return digitalRead(PIN_BOTAO_A) == LOW &&
         digitalRead(PIN_BOTAO_B) == LOW;
}
bool amblosSoltos() {
  return digitalRead(PIN_BOTAO_A) == HIGH &&
         digitalRead(PIN_BOTAO_B) == HIGH;
}

// ============================================================
void acionarPrensa() {
  digitalWrite(PIN_PRENSA, HIGH);
  digitalWrite(PIN_ERRO,   LOW);
  Serial.println("  ✔ PRENSA ATIVA — ambas as mãos confirmadas");
}

void pararPrensa() {
  digitalWrite(PIN_PRENSA, LOW);
  Serial.println("  ■ Prensa parada — botão solto");
}

void acionarErro(const char* motivo) {
  digitalWrite(PIN_PRENSA, LOW);
  digitalWrite(PIN_ERRO,   HIGH);
  tempoInicioErro = millis();
  estado = ERRO;
  Serial.print("  ✖ ERRO DE SEGURANÇA: ");
  Serial.println(motivo);
}

void resetarSistema() {
  digitalWrite(PIN_PRENSA, LOW);
  digitalWrite(PIN_ERRO,   LOW);
  primeiroBotao = 0;
  tempoprimeiro  = 0;
  estado = AGUARDANDO;
  Serial.println("  ○ Sistema resetado — aguardando comando\n");
}

// ============================================================
void loop() {
  unsigned long agora = millis();

  bool pressA = (digitalRead(PIN_BOTAO_A) == LOW);
  bool pressB = (digitalRead(PIN_BOTAO_B) == LOW);

  switch (estado) {

    // --------------------------------------------------------
    case AGUARDANDO:
      // Detecta qual botão foi pressionado primeiro
      if (pressA && !pressB) {
        delay(DEBOUNCE_MS);
        if (digitalRead(PIN_BOTAO_A) == LOW) {
          primeiroBotao = PIN_BOTAO_A;
          tempoprimeiro  = millis();
          estado = PRIMEIRO_BOTAO;
          Serial.println("  → Botão A pressionado — aguardando Botão B...");
        }
      } else if (pressB && !pressA) {
        delay(DEBOUNCE_MS);
        if (digitalRead(PIN_BOTAO_B) == LOW) {
          primeiroBotao = PIN_BOTAO_B;
          tempoprimeiro  = millis();
          estado = PRIMEIRO_BOTAO;
          Serial.println("  → Botão B pressionado — aguardando Botão A...");
        }
      } else if (pressA && pressB) {
        // Ambos simultaneamente: trata como válido
        delay(DEBOUNCE_MS);
        if (ambosPressos()) {
          acionarPrensa();
          estado = OPERANDO;
        }
      }
      break;

    // --------------------------------------------------------
    case PRIMEIRO_BOTAO: {
      unsigned long deltaT = agora - tempoprimeiro;

      // Janela expirou com apenas um botão → erro
      if (deltaT >= JANELA_MS) {
        Serial.print("  Δt = ");
        Serial.print(deltaT);
        Serial.println(" ms — janela expirada");
        acionarErro("apenas um botao pressionado (timeout)");
        break;
      }

      // Botão original foi solto antes do segundo → erro
      if (primeiroBotao == PIN_BOTAO_A && !pressA) {
        acionarErro("Botao A solto antes de pressionar B");
        break;
      }
      if (primeiroBotao == PIN_BOTAO_B && !pressB) {
        acionarErro("Botao B solto antes de pressionar A");
        break;
      }

      // Segundo botão pressionado dentro da janela → opera
      if (ambosPressos()) {
        delay(DEBOUNCE_MS);
        if (ambosPressos()) {
          Serial.print("  Δt = ");
          Serial.print(deltaT);
          Serial.println(" ms — dentro da janela ✔");
          acionarPrensa();
          estado = OPERANDO;
        }
      }
      break;
    }

    // --------------------------------------------------------
    case OPERANDO:
      // Qualquer botão solto → para a prensa imediatamente
      if (!pressA || !pressB) {
        delay(DEBOUNCE_MS);
        if (!ambosPressos()) {
          pararPrensa();
          estado = LIBERANDO;
        }
      }
      break;

    // --------------------------------------------------------
    case ERRO:
      // Mantém LED de erro por 2 s, depois aguarda ambos soltos
      if (agora - tempoInicioErro >= TEMPO_ERRO_MS) {
        digitalWrite(PIN_ERRO, LOW);
        Serial.println("  LED de erro apagado — solte os botões para resetar");
        estado = LIBERANDO;
      }
      break;

    // --------------------------------------------------------
    case LIBERANDO:
      // Aguarda ambos os botões serem completamente soltos
      if (amblosSoltos()) {
        delay(DEBOUNCE_MS);
        if (amblosSoltos()) {
          resetarSistema();
        }
      }
      break;
  }
}
