// ============================================================
//  Comparador Analógico com Dois Níveis
//  - Potenciômetro no pino A0
//  - LED no pino 5
//  - Referência de tensão: 5 V / 1023 steps
//
//  ≤ 2,0 V  → LED apagado
//  2,0–4,0V → LED piscando (500 ms)
//  ≥ 4,0 V  → LED aceso fixo
// ============================================================

const int   PIN_POT       = A0;
const int   PIN_LED       = 5;
const float TENSAO_REF    = 5.0;   // tensão de referência do Arduino (V)
const int   ADC_MAX       = 1023;

// Limites convertidos de tensão para valor ADC
// ADC = (tensão / TENSAO_REF) * ADC_MAX
const int NIVEL_1_ADC = (int)((2.0 / 5.0) * 1023); // ≈ 409  → 2,0 V
const int NIVEL_2_ADC = (int)((4.0 / 5.0) * 1023); // ≈ 818  → 4,0 V

const unsigned long BLINK_MS = 500; // intervalo de piscar (ms)

// --- Estado do piscar ---
bool          ledState    = false;
unsigned long ultimoBlink = 0;

// ============================================================
void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  Serial.begin(9600);

  Serial.println("╔══════════════════════════════╗");
  Serial.println("║  Comparador Analógico Pronto ║");
  Serial.println("╚══════════════════════════════╝");
  Serial.print("Nível 1 (2,0 V) = ADC ");
  Serial.println(NIVEL_1_ADC);
  Serial.print("Nível 2 (4,0 V) = ADC ");
  Serial.println(NIVEL_2_ADC);
  Serial.println();
}

// ============================================================
// Converte leitura ADC para tensão em volts
float adcParaTensao(int adc) {
  return (adc / (float)ADC_MAX) * TENSAO_REF;
}

// ============================================================
// Retorna uma barra visual proporcional à leitura (20 chars)
String barraVisual(int adc) {
  int pos = map(adc, 0, ADC_MAX, 0, 20);
  String barra = "[";
  for (int i = 0; i < 20; i++) {
    if (i < pos)       barra += "█";
    else if (i == pos) barra += "▌";
    else               barra += "·";
  }
  barra += "]";
  return barra;
}

// ============================================================
void loop() {
  int   leitura = analogRead(PIN_POT);
  float tensao  = adcParaTensao(leitura);

  // --- Zona 1: abaixo de 2,0 V → LED apagado ---
  if (leitura <= NIVEL_1_ADC) {
    digitalWrite(PIN_LED, LOW);
    ledState = false;

    Serial.print("[ ZONA 1 ] ");

  // --- Zona 2: entre 2,0 V e 4,0 V → LED piscando ---
  } else if (leitura < NIVEL_2_ADC) {
    unsigned long agora = millis();
    if (agora - ultimoBlink >= BLINK_MS) {
      ultimoBlink = agora;
      ledState    = !ledState;
      digitalWrite(PIN_LED, ledState);
    }

    Serial.print("[ ZONA 2 ] ");

  // --- Zona 3: acima de 4,0 V → LED aceso fixo ---
  } else {
    digitalWrite(PIN_LED, HIGH);
    ledState = true;

    Serial.print("[ ZONA 3 ] ");
  }

  // --- Log serial com tensão e barra visual ---
  Serial.print("ADC: ");
  Serial.print(leitura);
  Serial.print("  |  ");
  Serial.print(tensao, 2);
  Serial.print(" V  ");
  Serial.println(barraVisual(leitura));

  delay(100); // ~10 leituras/segundo
}
