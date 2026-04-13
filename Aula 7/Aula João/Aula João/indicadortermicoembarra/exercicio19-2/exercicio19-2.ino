// =============================================
//   Indicador Termico em Barra - NTC
// =============================================

const int ntcPin = A0;

// --- 8 LEDs da barra (pinos digitais) ---
const int NUM_LEDS = 8;
const int leds[NUM_LEDS] = {2, 3, 4, 5, 6, 7, 8, 9};

// --- Faixa de temperatura monitorada (°C) ---
const float TEMP_MIN = 20.0;  // 0 LEDs acesos
const float TEMP_MAX = 60.0;  // 8 LEDs acesos

// --- Parametros do NTC ---
const float BETA      = 3950.0;
const float R_NOMINAL = 10000.0;
const float T_NOMINAL = 25.0;
const float R_SERIE   = 10000.0;
const int   ADC_MAX   = 1023;

// --- Leitura estavel: media de 5 amostras ---
int lerADC() {
  long soma = 0;
  for (int i = 0; i < 5; i++) {
    soma += analogRead(ntcPin);
    delay(2);
  }
  return soma / 5;
}

// --- Converte ADC para temperatura Celsius ---
float adcParaTemp(int leitura) {
  if (leitura <= 0 || leitura >= ADC_MAX) return -999.0;
  float rNTC = R_SERIE * ((float)ADC_MAX / leitura - 1.0);
  float tempK = 1.0 / (
    (1.0 / (T_NOMINAL + 273.15)) +
    (1.0 / BETA) * log(rNTC / R_NOMINAL)
  );
  return tempK - 273.15;
}

// --- Atualiza a barra de LEDs ---
void atualizarBarra(int quantidade) {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(leds[i], i < quantidade ? HIGH : LOW);
  }
}

// --- Calcula quantos LEDs acender (0 a NUM_LEDS) ---
int calcularBarra(float temp) {
  if (temp <= TEMP_MIN) return 0;
  if (temp >= TEMP_MAX) return NUM_LEDS;
  float proporcao = (temp - TEMP_MIN) / (TEMP_MAX - TEMP_MIN);
  return (int)(proporcao * NUM_LEDS + 0.5); // arredonda
}

// --- Classifica a temperatura em zonas ---
const char* zonaTermica(float temp) {
  if (temp < TEMP_MIN)               return "FRIO    ";
  if (temp < TEMP_MIN + 10.0)        return "NORMAL  ";
  if (temp < TEMP_MAX - 10.0)        return "MORNO   ";
  if (temp < TEMP_MAX)               return "QUENTE  ";
  return                                    "CRITICO!";
}

// --- Barra ASCII no Serial ---
void imprimirSerial(float temp, int acesos) {
  Serial.print("Temp: ");
  if (temp < 10)  Serial.print("  ");
  else if (temp < 100) Serial.print(" ");
  Serial.print(temp, 1);
  Serial.print(" C | [");
  for (int i = 0; i < NUM_LEDS; i++) {
    Serial.print(i < acesos ? "#" : "-");
  }
  Serial.print("] ");
  Serial.print(acesos);
  Serial.print("/");
  Serial.print(NUM_LEDS);
  Serial.print(" | ");
  Serial.println(zonaTermica(temp));
}

void setup() {
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  Serial.begin(9600);
  Serial.println("=== Indicador Termico em Barra ===");
  Serial.print("Faixa: ");
  Serial.print(TEMP_MIN);
  Serial.print(" C (0 LEDs) ate ");
  Serial.print(TEMP_MAX);
  Serial.println(" C (8 LEDs)");
  Serial.println("----------------------------------");

  // Teste de inicializacao: acende todos em sequencia
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(leds[i], HIGH);
    delay(80);
  }
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    digitalWrite(leds[i], LOW);
    delay(80);
  }
}

void loop() {
  int   leitura = lerADC();
  float temp    = adcParaTemp(leitura);

  if (temp == -999.0) {
    // Sensor desconectado: pisca todos os LEDs
    for (int i = 0; i < NUM_LEDS; i++) digitalWrite(leds[i], HIGH);
    Serial.println("ERRO: Sensor NTC nao detectado!");
    delay(400);
    for (int i = 0; i < NUM_LEDS; i++) digitalWrite(leds[i], LOW);
    delay(400);
    return;
  }

  int acesos = calcularBarra(temp);
  atualizarBarra(acesos);
  imprimirSerial(temp, acesos);

  delay(300);
}
