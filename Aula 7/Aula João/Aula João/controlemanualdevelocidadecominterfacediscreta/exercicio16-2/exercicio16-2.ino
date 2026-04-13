const int botaoUp   = 2;  // Incrementa velocidade
const int botaoDown = 3;  // Decrementa velocidade
const int ledPWM    = 5;  // LED/Motor (PWM)
const int ledMin    = 6;  // Indicador limite minimo
const int ledMax    = 7;  // Indicador limite maximo

// --- Niveis discretos de velocidade (0 a 10) ---
const int NIVEIS     = 10;
const int PASSO      = 25;   // 255 / 10 = ~25 por nivel
const int VEL_MIN    = 0;
const int VEL_MAX    = 10;

// --- Debounce ---
const unsigned long DEBOUNCE_MS = 200;
unsigned long ultimoUp   = 0;
unsigned long ultimoDown = 0;

bool estadoUp   = false;
bool estadoDown = false;

int nivel = 0; // nivel atual (0 a 10)

// --- Converte nivel para PWM ---
int nivelParaPWM(int n) {
  return constrain(n * PASSO, 0, 255);
}

// --- Imprime barra de velocidade no Serial ---
void imprimirStatus(int n) {
  int pwm = nivelParaPWM(n);
  int pct = map(pwm, 0, 255, 0, 100);

  Serial.print("Nivel: ");
  Serial.print(n);
  Serial.print("/");
  Serial.print(NIVEIS);
  Serial.print(" | PWM: ");
  Serial.print(pwm);
  Serial.print(" | ");
  Serial.print(pct);
  Serial.print("% | [");
  for (int i = 0; i < NIVEIS; i++) {
    Serial.print(i < n ? "=" : " ");
  }
  Serial.print("]");

  if (n == VEL_MIN) Serial.print(" << MINIMO");
  if (n == VEL_MAX) Serial.print(" >> MAXIMO");
  Serial.println();
}

void setup() {
  pinMode(botaoUp,   INPUT_PULLUP);
  pinMode(botaoDown, INPUT_PULLUP);
  pinMode(ledPWM,    OUTPUT);
  pinMode(ledMin,    OUTPUT);
  pinMode(ledMax,    OUTPUT);

  analogWrite(ledPWM, 0);
  Serial.begin(9600);
  Serial.println("=== Controle de Velocidade ===");
  Serial.println("Botao UP = pino 2 | Botao DOWN = pino 3");
  Serial.println("-------------------------------------");
  imprimirStatus(nivel);
}

void loop() {
  bool leituraUp   = (digitalRead(botaoUp)   == LOW);
  bool leituraDown = (digitalRead(botaoDown) == LOW);
  unsigned long agora = millis();

  // --- Borda de subida com debounce ---
  if (leituraUp && !estadoUp && (agora - ultimoUp >= DEBOUNCE_MS)) {
    estadoUp  = true;
    ultimoUp  = agora;

    if (nivel < VEL_MAX) {
      nivel++;
      analogWrite(ledPWM, nivelParaPWM(nivel));
      imprimirStatus(nivel);
    } else {
      Serial.println("!! Velocidade maxima atingida.");
    }
  }
  if (!leituraUp) estadoUp = false;

  if (leituraDown && !estadoDown && (agora - ultimoDown >= DEBOUNCE_MS)) {
    estadoDown  = true;
    ultimoDown  = agora;

    if (nivel > VEL_MIN) {
      nivel--;
      analogWrite(ledPWM, nivelParaPWM(nivel));
      imprimirStatus(nivel);
    } else {
      Serial.println("!! Velocidade minima atingida.");
    }
  }
  if (!leituraDown) estadoDown = false;

  // --- LEDs de limite ---
  digitalWrite(ledMin, nivel == VEL_MIN ? HIGH : LOW);
  digitalWrite(ledMax, nivel == VEL_MAX ? HIGH : LOW);

  delay(10);
}
