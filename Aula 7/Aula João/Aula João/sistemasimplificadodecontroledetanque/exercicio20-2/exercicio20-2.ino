// =============================================
//   Controle de Tanque Industrial
// =============================================

const int potPin      = A0;  // Nivel do tanque
const int ntcPin      = A1;  // Temperatura
const int botaoEnable = 2;   // Habilitacao do sistema

const int ledValvula  = 4;   // LED1 - Valvula de entrada
const int ledAquecedor= 5;   // LED2 - Aquecedor (PWM)
const int ledAlarme   = 6;   // LED3 - Alarme

// --- Limites de nivel (0 a 100%) ---
const int NIVEL_BAIXO = 30;
const int NIVEL_ALTO  = 70;

// --- Limites de temperatura (°C) ---
const float TEMP_BAIXA   = 25.0;
const float TEMP_ALTA    = 60.0;
const float HISTERESE    = 2.0;

// --- Parametros NTC ---
const float BETA      = 3950.0;
const float R_NOMINAL = 10000.0;
const float T_NOMINAL = 25.0;
const float R_SERIE   = 10000.0;
const int   ADC_MAX   = 1023;

// --- Alarme piscante ---
const unsigned long PISCA_ALARME = 200;
unsigned long ultimoPisca = 0;
bool estadoPisca = false;

// --- Estado do aquecedor (histerese) ---
bool aquecedorLigado = false;

// --- Debounce botao ---
const unsigned long DEBOUNCE_MS = 200;
unsigned long ultimoBotao = 0;
bool estadoBotao  = false;
bool sistemaAtivo = false;

// --- Leitura estavel: media de 5 amostras ---
int lerADC(int pino) {
  long soma = 0;
  for (int i = 0; i < 5; i++) {
    soma += analogRead(pino);
    delay(2);
  }
  return soma / 5;
}

// --- Converte ADC para temperatura ---
float adcParaTemp(int leitura) {
  if (leitura <= 0 || leitura >= ADC_MAX) return -999.0;
  float rNTC = R_SERIE * ((float)ADC_MAX / leitura - 1.0);
  float tempK = 1.0 / (
    (1.0 / (T_NOMINAL + 273.15)) +
    (1.0 / BETA) * log(rNTC / R_NOMINAL)
  );
  return tempK - 273.15;
}

// --- Desliga todas as saidas ---
void desligarTudo() {
  digitalWrite(ledValvula,   LOW);
  analogWrite(ledAquecedor,  0);
  digitalWrite(ledAlarme,    LOW);
  aquecedorLigado = false;
}

// --- Controle da valvula por nivel ---
void controlarValvula(int nivel) {
  if (nivel < NIVEL_BAIXO) {
    digitalWrite(ledValvula, HIGH);   // nivel baixo: abre valvula
  } else if (nivel > NIVEL_ALTO) {
    digitalWrite(ledValvula, LOW);    // nivel alto: fecha valvula
  }
  // Entre os limites: mantem estado atual (histerese implicita)
}

// --- Controle do aquecedor por temperatura ---
void controlarAquecedor(float temp, int potencia) {
  if (!aquecedorLigado && temp < (TEMP_BAIXA - HISTERESE)) {
    aquecedorLigado = true;
  } else if (aquecedorLigado && temp > (TEMP_ALTA + HISTERESE)) {
    aquecedorLigado = false;
  }
  analogWrite(ledAquecedor, aquecedorLigado ? potencia : 0);
}

// --- Controle do alarme por temperatura alta ---
void controlarAlarme(float temp) {
  if (temp >= TEMP_ALTA) {
    unsigned long agora = millis();
    if (agora - ultimoPisca >= PISCA_ALARME) {
      estadoPisca = !estadoPisca;
      ultimoPisca = agora;
      digitalWrite(ledAlarme, estadoPisca ? HIGH : LOW);
    }
  } else {
    digitalWrite(ledAlarme, LOW);
    estadoPisca = false;
  }
}

// --- Imprime status no Serial ---
void imprimirStatus(int nivel, float temp, int potencia) {
  Serial.print("Sistema: ");
  Serial.print(sistemaAtivo ? "ATIVO   " : "INATIVO ");
  Serial.print("| Nivel: ");
  Serial.print(nivel);
  Serial.print("% ");
  if      (nivel < NIVEL_BAIXO) Serial.print("[BAIXO] ");
  else if (nivel > NIVEL_ALTO)  Serial.print("[ALTO]  ");
  else                          Serial.print("[NORMAL]");
  Serial.print("| Temp: ");
  Serial.print(temp, 1);
  Serial.print("C ");
  if      (temp < TEMP_BAIXA)   Serial.print("[FRIA]  ");
  else if (temp >= TEMP_ALTA)   Serial.print("[ALARME]");
  else                          Serial.print("[OK]    ");
  Serial.print("| Valvula: ");
  Serial.print(digitalRead(ledValvula) ? "ABERTA  " : "FECHADA ");
  Serial.print("| Aquec: ");
  Serial.print(aquecedorLigado ? potencia : 0);
  Serial.print("/255 | Alarme: ");
  Serial.println(temp >= TEMP_ALTA ? "ATIVO" : "---  ");
}

void setup() {
  pinMode(botaoEnable,  INPUT_PULLUP);
  pinMode(ledValvula,   OUTPUT);
  pinMode(ledAquecedor, OUTPUT);
  pinMode(ledAlarme,    OUTPUT);

  desligarTudo();
  Serial.begin(9600);
  Serial.println("=== Controle de Tanque Industrial ===");
  Serial.print("Nivel:  BAIXO < ");
  Serial.print(NIVEL_BAIXO);
  Serial.print("% | ALTO > ");
  Serial.print(NIVEL_ALTO);
  Serial.println("%");
  Serial.print("Temp:   FRIA < ");
  Serial.print(TEMP_BAIXA);
  Serial.print("C | ALARME > ");
  Serial.print(TEMP_ALTA);
  Serial.println("C");
  Serial.println("Pressione o botao para habilitar o sistema.");
  Serial.println("-------------------------------------");
}

void loop() {
  bool leitura = (digitalRead(botaoEnable) == LOW);
  unsigned long agora = millis();

  // --- Toggle do sistema pelo botao ---
  if (leitura && !estadoBotao && (agora - ultimoBotao >= DEBOUNCE_MS)) {
    estadoBotao  = true;
    ultimoBotao  = agora;
    sistemaAtivo = !sistemaAtivo;
    if (!sistemaAtivo) {
      desligarTudo();
      Serial.println(">> Sistema DESABILITADO.");
    } else {
      Serial.println(">> Sistema HABILITADO.");
    }
  }
  if (!leitura) estadoBotao = false;

  if (!sistemaAtivo) {
    delay(100);
    return;
  }

  // --- Leituras ---
  int   nivel    = map(lerADC(potPin), 0, 1023, 0, 100);
  float temp     = adcParaTemp(lerADC(ntcPin));
  int   potencia = 180; // potencia fixa do aquecedor (0~255)

  // Sensor NTC com falha
  if (temp == -999.0) {
    desligarTudo();
    digitalWrite(ledAlarme, HIGH);
    Serial.println("ERRO: Sensor NTC falhou! Sistema travado.");
    delay(500);
    return;
  }

  // --- Regras de controle ---
  controlarValvula(nivel);
  controlarAquecedor(temp, potencia);
  controlarAlarme(temp);

  // Temperatura alta: desliga aquecedor forcado
  if (temp >= TEMP_ALTA) {
    aquecedorLigado = false;
    analogWrite(ledAquecedor, 0);
  }

  imprimirStatus(nivel, temp, potencia);
  delay(300);
}
