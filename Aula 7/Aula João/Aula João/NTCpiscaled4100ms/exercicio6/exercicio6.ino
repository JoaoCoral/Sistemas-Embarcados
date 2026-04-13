// ============================================================
//  Alarme de Temperatura com Sensor NTC
//  - Sensor NTC no pino A0
//  - LED 4 no pino digital 6
//  - LED pisca a 100ms se temperatura > 40°C
// ============================================================

const int PIN_NTC = A0;       // Pino analógico do sensor NTC
const int PIN_LED4 = 6;       // Pino do LED 4

// --- Parâmetros da curva NTC (Steinhart-Hart simplificada) ---
const float BETA        = 3950.0;  // Coeficiente Beta do NTC
const float R_NOMINAL   = 10000.0; // Resistência nominal a 25°C (10kΩ)
const float T_NOMINAL   = 25.0;    // Temperatura nominal (°C)
const float R_SERIES    = 10000.0; // Resistor em série (divisor de tensão)

// --- Limiar de alarme ---
const float TEMP_ALARME = 40.0;    // Temperatura de disparo (°C)
const int   BLINK_DELAY = 100;     // Intervalo de piscar em ms

// --- Estado do LED ---
bool ledState = false;
unsigned long lastBlink = 0;

// ============================================================
void setup() {
  pinMode(PIN_LED4, OUTPUT);
  Serial.begin(9600);
}

// ============================================================
float lerTemperatura() {
  int leituraADC = analogRead(PIN_NTC);

  // Calcula a resistência do NTC pelo divisor de tensão
  float resistencia = R_SERIES * ((1023.0 / leituraADC) - 1.0);

  // Equação de Steinhart-Hart simplificada (1/T = 1/T0 + 1/B * ln(R/R0))
  float tempKelvin = 1.0 / (
    (1.0 / (T_NOMINAL + 273.15)) +
    (1.0 / BETA) * log(resistencia / R_NOMINAL)
  );

  return tempKelvin - 273.15; // Converte para Celsius
}

// ============================================================
void loop() {
  float temperatura = lerTemperatura();

  Serial.print("Temperatura: ");
  Serial.print(temperatura, 1);
  Serial.println(" °C");

  if (temperatura > TEMP_ALARME) {
    // ALARME: pisca o LED 4 a cada 100ms (sem usar delay)
    unsigned long agora = millis();
    if (agora - lastBlink >= BLINK_DELAY) {
      lastBlink = agora;
      ledState = !ledState;
      digitalWrite(PIN_LED4, ledState);
    }
  } else {
    // Temperatura normal: LED 4 apagado
    ledState = false;
    digitalWrite(PIN_LED4, LOW);
  }
}
