// ============================================================
//  Controle de Velocidade de Esteira com Emergencia
//  - Potenciometro no pino A0  -> velocidade do motor
//  - LED 1 no pino PWM 9       -> motor principal
//  - LED 2 no pino 8           -> status / falha critica
//  - Botao A no pino 2         -> emergencia / reset
// ============================================================

const int PIN_POT      = A0;
const int PIN_MOTOR    = 9;   // PWM
const int PIN_STATUS   = 8;
const int PIN_BOTAO_A  = 2;

const unsigned long BLINK_EMERGENCIA_MS = 150;  // pisca rapido — falha critica
const unsigned long LEITURA_MS          = 50;   // atualiza motor a cada 50ms

enum Estado { OPERANDO, EMERGENCIA };
Estado estado = OPERANDO;

unsigned long ultimaLeitura = 0;
unsigned long ultimoBlink   = 0;
bool          ledStatus     = false;
bool          anteriorA     = HIGH;

// ============================================================
void setup() {
  pinMode(PIN_MOTOR,   OUTPUT);
  pinMode(PIN_STATUS,  OUTPUT);
  pinMode(PIN_BOTAO_A, INPUT_PULLUP);

  analogWrite (PIN_MOTOR,  0);
  digitalWrite(PIN_STATUS, LOW);

  Serial.begin(9600);
  Serial.println("Sistema OPERANDO. Gire o potenciometro.");
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
String barraVelocidade(int pwm) {
  int pos = map(pwm, 0, 255, 0, 20);
  String b = "[";
  for (int i = 0; i < 20; i++) {
    if      (i < pos && pos >= 18) b += "X";
    else if (i < pos && pos >= 12) b += "=";
    else if (i < pos)              b += "-";
    else                           b += ".";
  }
  b += "]";
  return b;
}

// ============================================================
void loop() {
  unsigned long agora = millis();
  bool bA = clicouA();

  switch (estado) {

    // --------------------------------------------------------
    case OPERANDO:
      // Botao A: aciona emergencia
      if (bA) {
        analogWrite (PIN_MOTOR,  0);
        digitalWrite(PIN_STATUS, LOW);
        ledStatus = false;
        ultimoBlink = agora;
        estado = EMERGENCIA;
        Serial.println("");
        Serial.println("!!! EMERGENCIA ACIONADA !!!");
        Serial.println("Motor PARADO. Pressione Botao A para resetar.");
        break;
      }

      // Atualiza velocidade do motor pelo potenciometro
      if (agora - ultimaLeitura >= LEITURA_MS) {
        ultimaLeitura = agora;

        int   pot  = analogRead(PIN_POT);
        int   pwm  = map(pot, 0, 1023, 0, 255);
        float pct  = (pwm / 255.0) * 100.0;

        analogWrite(PIN_MOTOR, pwm);

        // LED de status: aceso fixo durante operacao normal
        digitalWrite(PIN_STATUS, HIGH);

        Serial.print("Velocidade: ");
        Serial.print((int)pct);
        Serial.print("%  ");
        Serial.print(barraVelocidade(pwm));
        Serial.print("  POT: ");
        Serial.print(pot);
        Serial.print("  PWM: ");
        Serial.println(pwm);
      }
      break;

    // --------------------------------------------------------
    case EMERGENCIA:
      // Piscar LED de status rapido — falha critica
      if (agora - ultimoBlink >= BLINK_EMERGENCIA_MS) {
        ultimoBlink = agora;
        ledStatus   = !ledStatus;
        digitalWrite(PIN_STATUS, ledStatus ? HIGH : LOW);
      }

      // Motor sempre parado em emergencia
      analogWrite(PIN_MOTOR, 0);

      // Botao A: reinicia o sistema
      if (bA) {
        digitalWrite(PIN_STATUS, HIGH);
        ledStatus = false;
        estado = OPERANDO;
        Serial.println("");
        Serial.println("Sistema REINICIADO. Operando normalmente.");
      }
      break;
  }
}
