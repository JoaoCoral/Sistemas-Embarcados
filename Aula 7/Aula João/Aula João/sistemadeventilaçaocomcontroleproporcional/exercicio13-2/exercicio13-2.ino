// =============================================
//   Ventilação Industrial - Controle Proporcional
// =============================================

const int ntcPin    = A0;  // Sensor NTC
const int ledPWM    = 3;   // LED/Ventilador (PWM)
const int ledAlerta = 4;   // LED - Alerta de temperatura crítica

// --- Faixa de operação (°C) ---
const float TEMP_MIN     = 20.0;  // Abaixo disso: ventilador desligado
const float TEMP_MAX     = 40.0;  // Acima disso: ventilador a 100%
const float TEMP_CRITICA = 45.0;  // Alerta de temperatura crítica

// --- Parâmetros do NTC ---
const float BETA       = 3950.0;
const float R_NOMINAL  = 10000.0;
const float T_NOMINAL  = 25.0;
const float R_SERIE    = 10000.0;
const int   ADC_MAX    = 1023;

// --- Filtro de média móvel ---
const int   N_AMOSTRAS = 10;
float       amostras[N_AMOSTRAS];
int         idxAmostra = 0;
bool        bufferCheio = false;

// --- Converte leitura ADC → temperatura °C ---
float adcParaTemp(int leitura) {
  if (leitura <= 0 || leitura >= ADC_MAX) return -999.0;

  float rNTC = R_SERIE * ((float)ADC_MAX / leitura - 1.0);
  float tempK = 1.0 / (
    (1.0 / (T_NOMINAL + 273.15)) +
    (1.0 / BETA) * log(rNTC / R_NOMINAL)
  );
  return tempK - 273.15;
}

// --- Média móvel para estabilizar leitura ---
float filtrarTemperatura(float novaTemp) {
  amostras[idxAmostra] = novaTemp;
  idxAmostra = (idxAmostra + 1) % N_AMOSTRAS;
  if (idxAmostra == 0) bufferCheio = true;

  int total = bufferCheio ? N_AMOSTRAS : idxAmostra;
  float soma = 0;
  for (int i = 0; i < total; i++) soma += amostras[i];
  return soma / total;
}

// --- Controle proporcional: temperatura → PWM ---
int calcularPWM(float temp) {
  if (temp <= TEMP_MIN) return 0;
  if (temp >= TEMP_MAX) return 255;

  // Mapeamento linear proporcional
  float proporcao = (temp - TEMP_MIN) / (TEMP_MAX - TEMP_MIN);
  return (int)(proporcao * 255.0);
}

void setup() {
  pinMode(ledPWM,    OUTPUT);
  pinMode(ledAlerta, OUTPUT);
  Serial.begin(9600);

  // Pré-aquece o buffer com leituras reais
  Serial.println("Inicializando sensor...");
  for (int i = 0; i < N_AMOSTRAS; i++) {
    float t = adcParaTemp(analogRead(ntcPin));
    if (t != -999.0) filtrarTemperatura(t);
    delay(50);
  }

  Serial.println("=== Sistema de Ventilacao Industrial ===");
  Serial.print("Faixa de operacao: ");
  Serial.print(TEMP_MIN); Serial.print(" C - ");
  Serial.print(TEMP_MAX); Serial.println(" C");
  Serial.println("----------------------------------------");
  Serial.println("Temp(C) | PWM | Ventilador | Status");
  Serial.println("--------+-----+------------+----------");
}

void loop() {
  int   leitura  = analogRead(ntcPin);
  float tempBruta = adcParaTemp(leitura);

  if (tempBruta == -999.0) {
    Serial.println("ERRO: Sensor NTC nao detectado!");
    analogWrite(ledPWM, 0);
    digitalWrite(ledAlerta, HIGH);
    delay(1000);
    return;
  }

  float temp = filtrarTemperatura(tempBruta);
  int   pwm  = calcularPWM(temp);
  int   pct  = map(pwm, 0, 255, 0, 100);

  analogWrite(ledPWM, pwm);

  // Alerta de temperatura crítica
  bool critico = (temp >= TEMP_CRITICA);
  digitalWrite(ledAlerta, critico ? HIGH : LOW);

  // Status textual
  String status;
  if      (temp < TEMP_MIN)    status = "DESLIGADO ";
  else if (temp >= TEMP_CRITICA) status = "CRITICO!  ";
  else if (pct < 50)           status = "BAIXA VEL.";
  else if (pct < 80)           status = "MEDIA VEL.";
  else                         status = "ALTA VEL. ";

  // Serial formatado
  Serial.print(temp, 1);
  Serial.print("°C   | ");
  Serial.print(pwm);
  Serial.print("  | ");
  Serial.print(pct);
  Serial.print("%        | ");
  Serial.println(status);

  delay(200);
}
