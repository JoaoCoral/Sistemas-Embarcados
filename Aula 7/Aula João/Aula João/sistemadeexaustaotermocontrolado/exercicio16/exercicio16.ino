// ============================================================
//  Sistema de Exaustão Termo-Controlado — Túnel de Pintura
//  - Sensor NTC no pino A0 (divisor de tensão com 10kΩ)
//  - LED 2 no pino PWM 6  → Exaustor (brilho = potência)
//  - LED 1 no pino 5      → Indicador "Capacidade Máxima"
// ============================================================

const int PIN_NTC       = A0;
const int PIN_EXAUSTOR  = 6;   // PWM — LED 2
const int PIN_CAP_MAX   = 5;   // Digital — LED 1

// --- Parâmetros do NTC (Steinhart-Hart simplificada) ---
const float BETA       = 3950.0;
const float R_NOMINAL  = 10000.0;  // 10 kΩ a 25°C
const float T_NOMINAL  = 25.0;     // °C
const float R_SERIE    = 10000.0;  // resistor em série (divisor)

// --- Faixas de operação ---
const float TEMP_BAIXA  = 30.0;   // abaixo → 20% fixo
const float TEMP_ALTA   = 60.0;   // acima  → 100% + alerta

// --- Limites de potência em PWM (0–255) ---
const int PWM_MINIMO  = (int)(0.20 * 255);  // 20%  →  51
const int PWM_MAXIMO  = 255;                // 100% → 255

// --- Intervalo entre leituras ---
const unsigned long LEITURA_MS = 500;
unsigned long ultimaLeitura    = 0;

// ============================================================
void setup() {
  pinMode(PIN_EXAUSTOR, OUTPUT);
  pinMode(PIN_CAP_MAX,  OUTPUT);
  analogWrite (PIN_EXAUSTOR, PWM_MINIMO);
  digitalWrite(PIN_CAP_MAX,  LOW);

  Serial.begin(9600);
  Serial.println("╔══════════════════════════════════════════════╗");
  Serial.println("║   Exaustão Termo-Controlada — Túnel Pintura  ║");
  Serial.println("╠══════════════════════════════════════════════╣");
  Serial.println("║  < 30°C          →  20% de potência          ║");
  Serial.println("║  30°C – 60°C     →  20% a 100% (linear)      ║");
  Serial.println("║  > 60°C          →  100% + alerta CAP. MÁX   ║");
  Serial.println("╚══════════════════════════════════════════════╝\n");
}

// ============================================================
float lerTemperatura() {
  int   adc = analogRead(PIN_NTC);

  // Proteção contra divisão por zero
  if (adc <= 0)    return -99.0;
  if (adc >= 1023) return  999.0;

  float resistencia = R_SERIE * ((1023.0 / adc) - 1.0);
  float tempK = 1.0 / (
    (1.0 / (T_NOMINAL + 273.15)) +
    (1.0 / BETA) * log(resistencia / R_NOMINAL)
  );
  return tempK - 273.15;
}

// ============================================================
int calcularPWM(float temp) {
  if (temp < TEMP_BAIXA) {
    return PWM_MINIMO;

  } else if (temp <= TEMP_ALTA) {
    // Interpolação linear: 30°C→51 PWM / 60°C→255 PWM
    float fator = (temp - TEMP_BAIXA) / (TEMP_ALTA - TEMP_BAIXA);
    return (int)(PWM_MINIMO + fator * (PWM_MAXIMO - PWM_MINIMO));

  } else {
    return PWM_MAXIMO;
  }
}

// ============================================================
String barraVisual(int pwm) {
  int pos = map(pwm, 0, 255, 0, 20);
  String b = "[";
  for (int i = 0; i < 20; i++) {
    if      (i < pos && pos >= 20)  b += "█";  // 100%
    else if (i < pos && pos >= 14)  b += "▓";  // >70%
    else if (i < pos)               b += "░";  // normal
    else                            b += "·";
  }
  b += "]";
  return b;
}

// ============================================================
void loop() {
  if (millis() - ultimaLeitura < LEITURA_MS) return;
  ultimaLeitura = millis();

  float temp = lerTemperatura();
  int   pwm  = calcularPWM(temp);
  float pct  = (pwm / 255.0) * 100.0;

  analogWrite(PIN_EXAUSTOR, pwm);

  // --- LED 1: "Capacidade Máxima" apenas acima de 60°C ---
  bool capMax = (temp > TEMP_ALTA);
  digitalWrite(PIN_CAP_MAX, capMax ? HIGH : LOW);

  // --- Log serial ---
  Serial.print("Temp: ");
  if (temp > -90.0 && temp < 900.0) {
    Serial.print(temp, 1);
    Serial.print(" °C");
  } else {
    Serial.print("(sensor erro)");
  }

  Serial.print("  |  PWM: ");
  Serial.print(pwm);
  Serial.print(" (");
  Serial.print(pct, 1);
  Serial.print("%)  ");
  Serial.print(barraVisual(pwm));

  if (temp < TEMP_BAIXA) {
    Serial.println("  [ ZONA 1 — mínimo     ]");
  } else if (temp <= TEMP_ALTA) {
    Serial.println("  [ ZONA 2 — proporcional]");
  } else {
    Serial.println("  [ ZONA 3 — CAP. MÁXIMA ]");
  }
}
