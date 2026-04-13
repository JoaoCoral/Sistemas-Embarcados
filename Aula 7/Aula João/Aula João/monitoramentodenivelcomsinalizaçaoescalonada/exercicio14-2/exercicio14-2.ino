// =============================================
//   Monitoramento de Nível - Reservatório
// =============================================

const int potPin      = A0;
const int ledVerde    = 2;   // Nível médio  - Normal
const int ledAmarelo  = 3;   // Nível baixo  - Atenção
const int ledVermelho = 4;   // Nível alto   - Atenção
const int ledAlerta   = 5;   // Nível crítico - Pisca

// --- Faixas de nível (%) ---
const int NIVEL_CRITICO_BAIXO = 10;
const int NIVEL_BAIXO         = 25;
const int NIVEL_ALTO          = 75;
const int NIVEL_CRITICO_ALTO  = 90;

// --- Classificações com #define (evita enum class) ---
#define CRITICO_BAIXO 0
#define BAIXO         1
#define MEDIO         2
#define ALTO          3
#define CRITICO_ALTO  4

// --- Pisca sem delay ---
const unsigned long INTERVALO_PISCA = 300;
unsigned long ultimoPisca = 0;
bool estadoPisca = false;

// --- Lê potenciômetro e converte para 0~100% ---
int lerNivel() {
  return map(analogRead(potPin), 0, 1023, 0, 100);
}

// --- Classifica o nível ---
int classificar(int pct) {
  if (pct < NIVEL_CRITICO_BAIXO) return CRITICO_BAIXO;
  if (pct < NIVEL_BAIXO)         return BAIXO;
  if (pct > NIVEL_CRITICO_ALTO)  return CRITICO_ALTO;
  if (pct > NIVEL_ALTO)          return ALTO;
  return MEDIO;
}

// --- Apaga todos os LEDs ---
void desligarTodos() {
  digitalWrite(ledVerde,    LOW);
  digitalWrite(ledAmarelo,  LOW);
  digitalWrite(ledVermelho, LOW);
  digitalWrite(ledAlerta,   LOW);
  estadoPisca = false;
}

// --- Atualiza LEDs conforme classificação ---
void atualizarLEDs(int nivel) {
  digitalWrite(ledVerde,    LOW);
  digitalWrite(ledAmarelo,  LOW);
  digitalWrite(ledVermelho, LOW);

  switch (nivel) {
    case CRITICO_BAIXO:
      digitalWrite(ledAmarelo, HIGH);
      if (millis() - ultimoPisca >= INTERVALO_PISCA) {
        estadoPisca = !estadoPisca;
        ultimoPisca = millis();
        digitalWrite(ledAlerta, estadoPisca ? HIGH : LOW);
      }
      break;

    case BAIXO:
      digitalWrite(ledAmarelo, HIGH);
      digitalWrite(ledAlerta,  LOW);
      break;

    case MEDIO:
      digitalWrite(ledVerde,  HIGH);
      digitalWrite(ledAlerta, LOW);
      break;

    case ALTO:
      digitalWrite(ledVermelho, HIGH);
      digitalWrite(ledAlerta,   LOW);
      break;

    case CRITICO_ALTO:
      digitalWrite(ledVermelho, HIGH);
      if (millis() - ultimoPisca >= INTERVALO_PISCA) {
        estadoPisca = !estadoPisca;
        ultimoPisca = millis();
        digitalWrite(ledAlerta, estadoPisca ? HIGH : LOW);
      }
      break;
  }
}

// --- Texto do status ---
const char* textoNivel(int nivel) {
  switch (nivel) {
    case CRITICO_BAIXO: return "CRITICO BAIXO";
    case BAIXO:         return "BAIXO        ";
    case MEDIO:         return "MEDIO        ";
    case ALTO:          return "ALTO         ";
    case CRITICO_ALTO:  return "CRITICO ALTO ";
    default:            return "---          ";
  }
}

// --- Barra visual no Serial ---
void imprimirBarra(int pct) {
  Serial.print("[");
  for (int i = 0; i < 20; i++) {
    Serial.print(i < (pct / 5) ? "=" : " ");
  }
  Serial.print("]");
}

void setup() {
  pinMode(potPin,      INPUT);
  pinMode(ledVerde,    OUTPUT);
  pinMode(ledAmarelo,  OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAlerta,   OUTPUT);

  desligarTodos();
  Serial.begin(9600);
  Serial.println("=== Monitor de Nivel do Reservatorio ===");
  Serial.println("Nivel  | Barra                | Status        | Alerta");
  Serial.println("-------+----------------------+---------------+-------");
}

void loop() {
  int pct   = lerNivel();
  int nivel = classificar(pct);

  atualizarLEDs(nivel);

  if (pct < 10) Serial.print(" ");
  Serial.print(pct);
  Serial.print("%    | ");
  imprimirBarra(pct);
  Serial.print(" | ");
  Serial.print(textoNivel(nivel));
  Serial.print(" | ");
  Serial.println(
    (nivel == CRITICO_BAIXO || nivel == CRITICO_ALTO)
    ? "*** PISCA ***"
    : "---"
  );

  delay(100);
}
