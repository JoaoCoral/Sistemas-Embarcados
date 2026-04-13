// ============================================================
//  Esteira de Triagem de Lotes — versao Tinkercad
// ============================================================

const int PIN_BOTAO_A = 2;
const int PIN_BOTAO_B = 3;
const int PIN_ESTEIRA = 5;
const int PIN_BRACO   = 6;
const int PIN_LOTE    = 7;
const int PIN_MAX     = 8;

const int CAIXAS_POR_LOTE    = 5;
const int CAIXAS_MAXIMAS     = 20;
const unsigned long PAUSA_MS = 3000;
const unsigned long BLINK_MS = 300;

enum Estado { PARADO, MOVENDO, PAUSADO, CAPACIDADE };
Estado estado = PARADO;

int           caixasLote  = 0;
int           caixasTotal = 0;
unsigned long inicioPausa = 0;
unsigned long ultimoBlink = 0;
bool          ledLote     = false;

bool anteriorA = HIGH;
bool anteriorB = HIGH;

// ============================================================
void setup() {
  pinMode(PIN_BOTAO_A, INPUT_PULLUP);
  pinMode(PIN_BOTAO_B, INPUT_PULLUP);
  pinMode(PIN_ESTEIRA, OUTPUT);
  pinMode(PIN_BRACO,   OUTPUT);
  pinMode(PIN_LOTE,    OUTPUT);
  pinMode(PIN_MAX,     OUTPUT);

  digitalWrite(PIN_ESTEIRA, LOW);
  digitalWrite(PIN_BRACO,   LOW);
  digitalWrite(PIN_LOTE,    LOW);
  digitalWrite(PIN_MAX,     LOW);

  Serial.begin(9600);
  Serial.println("Pronto. Pressione Botao A para iniciar.");
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

bool clicouB() {
  bool leitura = digitalRead(PIN_BOTAO_B);
  if (leitura == LOW && anteriorB == HIGH) {
    delay(30);
    anteriorB = digitalRead(PIN_BOTAO_B);
    return (anteriorB == LOW);
  }
  anteriorB = leitura;
  return false;
}

// ============================================================
void imprimirStatus() {
  Serial.print("Lote: ");
  Serial.print(caixasLote);
  Serial.print("/");
  Serial.print(CAIXAS_POR_LOTE);
  Serial.print(" | Total: ");
  Serial.print(caixasTotal);
  Serial.print("/");
  Serial.println(CAIXAS_MAXIMAS);
}

// ============================================================
void loop() {
  unsigned long agora = millis();
  bool bA = clicouA();
  bool bB = clicouB();

  // Piscar LED 3 durante pausa
  if (estado == PAUSADO) {
    if (agora - ultimoBlink >= BLINK_MS) {
      ultimoBlink = agora;
      ledLote = !ledLote;
      digitalWrite(PIN_LOTE, ledLote ? HIGH : LOW);
    }
  }

  switch (estado) {

    case PARADO:
      if (bA) {
        caixasLote  = 0;
        caixasTotal = 0;
        digitalWrite(PIN_ESTEIRA, HIGH);
        digitalWrite(PIN_MAX,     LOW);
        estado = MOVENDO;
        Serial.println("Esteira LIGADA.");
      }
      break;

    case MOVENDO:
      if (bB) {
        caixasLote++;
        caixasTotal++;
        Serial.print("Caixa detectada! ");
        imprimirStatus();

        if (caixasTotal >= CAIXAS_MAXIMAS) {
          digitalWrite(PIN_ESTEIRA, LOW);
          digitalWrite(PIN_MAX,     HIGH);
          estado = CAPACIDADE;
          Serial.println("CAPACIDADE MAXIMA atingida! Pressione Botao A para resetar.");
          break;
        }

        if (caixasLote >= CAIXAS_POR_LOTE) {
          caixasLote = 0;
          digitalWrite(PIN_ESTEIRA, LOW);
          digitalWrite(PIN_BRACO,   HIGH);
          inicioPausa = agora;
          ultimoBlink = agora;
          ledLote     = false;
          estado      = PAUSADO;
          Serial.println("Lote completo! Braco acionado. Pausando 3 segundos...");
        }
      }
      break;

    case PAUSADO:
      if (agora - inicioPausa >= PAUSA_MS) {
        digitalWrite(PIN_BRACO, LOW);
        digitalWrite(PIN_LOTE,  LOW);
        ledLote = false;
        digitalWrite(PIN_ESTEIRA, HIGH);
        estado = MOVENDO;
        Serial.println("Braco recolhido. Esteira retomada.");
      }
      break;

    case CAPACIDADE:
      if (bA) {
        caixasLote  = 0;
        caixasTotal = 0;
        digitalWrite(PIN_ESTEIRA, LOW);
        digitalWrite(PIN_BRACO,   LOW);
        digitalWrite(PIN_LOTE,    LOW);
        digitalWrite(PIN_MAX,     LOW);
        ledLote = false;
        estado  = PARADO;
        Serial.println("Sistema resetado. Pressione Botao A para iniciar.");
      }
      break;
  }
}
