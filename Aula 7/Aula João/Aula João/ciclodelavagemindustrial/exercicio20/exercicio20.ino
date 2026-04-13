// ============================================================
//  Ciclo de Lavagem Industrial
//  - Botao A no pino 2   -> inicia ciclo
//  - LED 1 no pino 5     -> Enchimento
//  - LED 2 no pino 6     -> Lavagem (PWM 50%, alterna com LED 3)
//  - LED 3 no pino 9     -> Lavagem (PWM 50%, alterna com LED 2)
//  - LED 4 no pino 8     -> Drenagem
// ============================================================

const int PIN_BOTAO_A = 2;
const int PIN_LED1    = 5;   // Enchimento
const int PIN_LED2    = 6;   // Lavagem A — PWM
const int PIN_LED3    = 9;   // Lavagem B — PWM
const int PIN_LED4    = 8;   // Drenagem

const unsigned long TEMPO_ENCHIMENTO = 4000;  // 4 s
const unsigned long TEMPO_LAVAGEM    = 8000;  // 8 s
const unsigned long TEMPO_DRENAGEM   = 4000;  // 4 s
const unsigned long BLINK_LAVAGEM_MS = 400;   // intervalo de alternancia

const int PWM_LAVAGEM = 127;  // 50% de 255

enum Estado { AGUARDANDO, ENCHIMENTO, LAVAGEM, DRENAGEM };
Estado estado = AGUARDANDO;

unsigned long inicioEtapa = 0;
unsigned long ultimoAlterna = 0;
bool          faseAlterna   = false;
bool          anteriorA     = HIGH;

// ============================================================
void setup() {
  pinMode(PIN_BOTAO_A, INPUT_PULLUP);
  pinMode(PIN_LED1,    OUTPUT);
  pinMode(PIN_LED2,    OUTPUT);
  pinMode(PIN_LED3,    OUTPUT);
  pinMode(PIN_LED4,    OUTPUT);

  digitalWrite(PIN_LED1, LOW);
  analogWrite (PIN_LED2, 0);
  analogWrite (PIN_LED3, 0);
  digitalWrite(PIN_LED4, LOW);

  Serial.begin(9600);
  Serial.println("Sistema pronto. Pressione Botao A para iniciar o ciclo.");
}

// ============================================================
bool clicouA() {
  bool leitura = digitalRead(PIN_BOTAO_A);
  if (leitura == LOW && anteriorA == HIGH) {
    delay(30);
    anteriorA = digitalRead(PIN_BOTAO_A);
    return (anteriorA == LOW);
  }
  anteriorA = leitura;
  return false;
}

// ============================================================
void apagarTudo() {
  digitalWrite(PIN_LED1, LOW);
  analogWrite (PIN_LED2, 0);
  analogWrite (PIN_LED3, 0);
  digitalWrite(PIN_LED4, LOW);
}

// ============================================================
void iniciarEnchimento() {
  apagarTudo();
  digitalWrite(PIN_LED1, HIGH);
  inicioEtapa = millis();
  estado      = ENCHIMENTO;
  Serial.println("");
  Serial.println("ETAPA 1/3 — Enchimento iniciado (4s)");
}

void iniciarLavagem() {
  apagarTudo();
  faseAlterna  = false;
  ultimoAlterna = millis();
  inicioEtapa  = millis();
  estado       = LAVAGEM;
  Serial.println("ETAPA 2/3 — Lavagem iniciada (8s)");
}

void iniciarDrenagem() {
  apagarTudo();
  digitalWrite(PIN_LED4, HIGH);
  inicioEtapa = millis();
  estado      = DRENAGEM;
  Serial.println("ETAPA 3/3 — Drenagem iniciada (4s)");
}

// ============================================================
// Barra de progresso da etapa atual
void imprimirProgresso(unsigned long decorrido, unsigned long total) {
  int pos = map(decorrido, 0, total, 0, 20);
  pos = constrain(pos, 0, 20);
  Serial.print("[");
  for (int i = 0; i < 20; i++) {
    if (i < pos) Serial.print("=");
    else         Serial.print(".");
  }
  Serial.print("] ");
  int restante = (int)((total - decorrido) / 1000) + 1;
  Serial.print(restante);
  Serial.println("s");
}

// ============================================================
void loop() {
  unsigned long agora = millis();
  bool bA = clicouA();

  switch (estado) {

    // --------------------------------------------------------
    case AGUARDANDO:
      if (bA) {
        iniciarEnchimento();
      }
      break;

    // --------------------------------------------------------
    case ENCHIMENTO: {
      unsigned long decorrido = agora - inicioEtapa;

      // Progresso a cada segundo
      static unsigned long ultimoLog = 0;
      if (agora - ultimoLog >= 1000) {
        ultimoLog = agora;
        Serial.print("  Enchimento: ");
        imprimirProgresso(decorrido, TEMPO_ENCHIMENTO);
      }

      if (decorrido >= TEMPO_ENCHIMENTO) {
        iniciarLavagem();
      }
      break;
    }

    // --------------------------------------------------------
    case LAVAGEM: {
      unsigned long decorrido = agora - inicioEtapa;

      // Alternancia PWM entre LED 2 e LED 3
      if (agora - ultimoAlterna >= BLINK_LAVAGEM_MS) {
        ultimoAlterna = agora;
        faseAlterna   = !faseAlterna;

        if (faseAlterna) {
          analogWrite(PIN_LED2, PWM_LAVAGEM);
          analogWrite(PIN_LED3, 0);
        } else {
          analogWrite(PIN_LED2, 0);
          analogWrite(PIN_LED3, PWM_LAVAGEM);
        }
      }

      // Progresso a cada segundo
      static unsigned long ultimoLog = 0;
      if (agora - ultimoLog >= 1000) {
        ultimoLog = agora;
        Serial.print("  Lavagem:    ");
        imprimirProgresso(decorrido, TEMPO_LAVAGEM);
      }

      if (decorrido >= TEMPO_LAVAGEM) {
        iniciarDrenagem();
      }
      break;
    }

    // --------------------------------------------------------
    case DRENAGEM: {
      unsigned long decorrido = agora - inicioEtapa;

      // Progresso a cada segundo
      static unsigned long ultimoLog = 0;
      if (agora - ultimoLog >= 1000) {
        ultimoLog = agora;
        Serial.print("  Drenagem:   ");
        imprimirProgresso(decorrido, TEMPO_DRENAGEM);
      }

      if (decorrido >= TEMPO_DRENAGEM) {
        apagarTudo();
        estado = AGUARDANDO;
        Serial.println("");
        Serial.println("Ciclo concluido! Pressione Botao A para novo ciclo.");
      }
      break;
    }
  }
}
