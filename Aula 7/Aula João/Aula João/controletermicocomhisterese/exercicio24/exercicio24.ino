// =============================================
//   Controle Térmico com Histerese - NTC
// =============================================

const int ntcPin      = A0;  // Sensor NTC
const int ledAquec    = 3;   // LED - Elemento de aquecimento
const int ledStatus   = 4;   // LED - Sistema ativo (opcional)

// --- Limites de temperatura (°C) ---
const float TEMP_LIGA   = 22.0;  // Aciona aquecimento abaixo deste valor
const float TEMP_DESLIGA = 26.0; // Desliga aquecimento acima deste valor
// Histerese = TEMP_DESLIGA - TEMP_LIGA = 4°C

// --- Parâmetros do NTC (Steinhart-Hart) ---
const float BETA        = 3950.0; // Coeficiente B do NTC (ver datasheet)
const float R_NOMINAL   = 10000.0; // Resistência nominal a 25°C (10kΩ)
const float T_NOMINAL   = 25.0;    // Temperatura nominal (°C)
const float R_SERIE     = 10000.0; // Resistor em série com o NTC (10kΩ)
const int   ADC_MAX     = 1023;

bool aquecendo = false;

// --- Converte leitura ADC para temperatura em °C ---
float lerTemperatura() {
  int    leitura = analogRead(ntcPin);

  // Evita divisão por zero
  if (leitura <= 0 || leitura >= ADC_MAX) return -999.0;

  // Calcula resistência do NTC
  float rNTC = R_SERIE * ((float)ADC_MAX / leitura - 1.0);

  // Equação de Steinhart-Hart simplificada (Beta)
  float tempK = 1.0 / (
    (1.0 / (T_NOMINAL + 273.15)) +
    (1.0 / BETA) * log(rNTC / R_NOMINAL)
  );

  return tempK - 273.15; // Kelvin → Celsius
}

void setup() {
  pinMode(ledAquec,  OUTPUT);
  pinMode(ledStatus, OUTPUT);
  digitalWrite(ledStatus, HIGH); // Sistema ativo
  Serial.begin(9600);

  Serial.println("=== Controle Termico com Histerese ===");
  Serial.print("Faixa de controle: ");
  Serial.print(TEMP_LIGA);
  Serial.print(" C (liga) / ");
  Serial.print(TEMP_DESLIGA);
  Serial.println(" C (desliga)");
  Serial.println("--------------------------------------");
}

void loop() {
  float temp = lerTemperatura();

  // Leitura inválida (sensor desconectado)
  if (temp == -999.0) {
    Serial.println("ERRO: Sensor NTC nao detectado!");
    digitalWrite(ledAquec, LOW);
    delay(1000);
    return;
  }

  // --- Lógica de histerese ---
  if (!aquecendo && temp < TEMP_LIGA) {
    aquecendo = true;
    digitalWrite(ledAquec, HIGH);
    Serial.println(">> Aquecimento LIGADO");
  } else if (aquecendo && temp > TEMP_DESLIGA) {
    aquecendo = false;
    digitalWrite(ledAquec, LOW);
    Serial.println(">> Aquecimento DESLIGADO");
  }

  // --- Serial Monitor formatado ---
  Serial.print("Temp: ");
  Serial.print(temp, 1);
  Serial.print(" C | Aquecimento: ");
  Serial.print(aquecendo ? "ON " : "OFF");
  Serial.print(" | Estado: ");

  if (temp < TEMP_LIGA)
    Serial.println("ABAIXO DO LIMITE");
  else if (temp > TEMP_DESLIGA)
    Serial.println("ACIMA DO LIMITE");
  else
    Serial.println("NA FAIXA (histerese)");

  delay(500);
}
