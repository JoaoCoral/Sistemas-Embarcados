const int potPin      = A0;
const int ledVerde    = 2;  // Estado normal
const int ledVermelho = 3;  // Estado de alerta
const int ledAzul     = 4;  // Pisca junto no alerta (alta frequencia)

// --- Limites ---
const int LIMITE_CRITICO = 700;  // 0~1023 (~170V em escala de 250V)
const int HISTERESE      = 50;   // evita oscilacao na borda do limite

// --- Pisca do alerta ---
const unsigned long PISCA_NORMAL  = 500; // ms (estado normal - lento)
const unsigned long PISCA_ALERTA  = 100; // ms (estado critico - rapido)

unsigned long ultimoPisca = 0;
bool estadoPisca = false;

// --- Maquina de estados ---
#define ESTADO_NORMAL 0
#define ESTADO_ALERTA 1
int estado = ESTADO_NORMAL;

// --- Leitura estavel com media de 5 amostras ---
int lerSinal() {
  long soma = 0;
  for (int i = 0; i < 5; i++) {
    soma += analogRead(potPin);
    delay(2);
  }
  return soma / 5;
}

void desligarTodos() {
  digitalWrite(ledVerde,    LOW);
  digitalWrite(ledVermelho, LOW);
  digitalWrite(ledAzul,     LOW);
}

void setup() {
  pinMode(ledVerde,    OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAzul,     OUTPUT);
  desligarTodos();
  Serial.begin(9600);
  Serial.println("=== Sistema de Alarme ===");
  Serial.print("Limite critico: ");
  Serial.print(map(LIMITE_CRITICO, 0, 1023, 0, 100));
  Serial.println("%");
  Serial.println("-------------------------");
}

void loop() {
  int  leitura = lerSinal();
  int  pct     = map(leitura, 0, 1023, 0, 100);
  unsigned long agora = millis();

  // --- Transicao de estados com histerese ---
  if (estado == ESTADO_NORMAL && leitura > LIMITE_CRITICO) {
    estado = ESTADO_ALERTA;
    Serial.println(">> ALERTA: valor critico atingido!");
  } else if (estado == ESTADO_ALERTA && leitura < (LIMITE_CRITICO - HISTERESE)) {
    estado = ESTADO_NORMAL;
    Serial.println(">> NORMAL: valor normalizado.");
  }

  // --- Controle dos LEDs por estado ---
  desligarTodos();
  unsigned long intervalo = (estado == ESTADO_ALERTA) ? PISCA_ALERTA : PISCA_NORMAL;

  if (agora - ultimoPisca >= intervalo) {
    estadoPisca = !estadoPisca;
    ultimoPisca = agora;
  }

  if (estado == ESTADO_NORMAL) {
    digitalWrite(ledVerde, HIGH);        // verde fixo
    digitalWrite(ledAzul,  LOW);
    digitalWrite(ledVermelho, LOW);

  } else {
    // Alerta: vermelho e azul piscam alternados em alta frequencia
    digitalWrite(ledVerde,    LOW);
    digitalWrite(ledVermelho, estadoPisca  ? HIGH : LOW);
    digitalWrite(ledAzul,     !estadoPisca ? HIGH : LOW);
  }

  // --- Serial Monitor ---
  Serial.print("Sinal: ");
  Serial.print(pct);
  Serial.print("% (");
  Serial.print(leitura);
  Serial.print(") | Estado: ");
  Serial.print(estado == ESTADO_NORMAL ? "NORMAL " : "ALERTA!");
  Serial.print(" | [");
  for (int i = 0; i < 20; i++) {
    if (i == map(LIMITE_CRITICO, 0, 1023, 0, 20)) Serial.print("|");
    else Serial.print(i < (pct / 5) ? "=" : " ");
  }
  Serial.println("]");

  delay(50);
}
