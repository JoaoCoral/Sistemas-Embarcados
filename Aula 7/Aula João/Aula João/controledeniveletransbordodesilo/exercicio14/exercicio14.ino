// ============================================================
//  Controle de Nível e Transbordo de Silo
//  - Sensor de distância potenciométrico no pino A0
//    (0 = silo vazio / 1023 = silo cheio)
//  - LED 1 no pino 5  → Válvula de carga (HIGH = aberta)
//  - LED 2 no pino 6  → Alarme de transbordo (pisca em emergência)
// ============================================================

const int PIN_SENSOR = A0;
const int PIN_VALVULA = 5;   // LED 1 — válvula de carga
const int PIN_ALARME  = 6;   // LED 2 — alarme de transbordo

const int ADC_MAX = 1023;

// --- Limiares convertidos de % para ADC ---
const int NIVEL_REABERTURA   = (int)(0.30 * ADC_MAX);  // 30%  → 306
const int NIVEL_FECHAMENTO   = (int)(0.80 * ADC_MAX);  // 80%  → 818
const int NIVEL_TRANSBORDO   = (int)(0.95 * ADC_MAX);  // 95%  → 971

const unsigned long BLINK_ALARME_MS = 200;  // pisca rápido — emergência
const unsigned long LEITURA_MS      = 200;  // intervalo entre leituras

// --- Máquina de estados do silo ---
enum EstadoSilo {
  CARREGANDO,     // válvula aberta, enchendo normalmente
  CHEIO,          // válvula fechada (atingiu 80%), aguardando esvaziamento
  TRANSBORDO      // válvula fechada + alarme piscando (atingiu 95%)
};

EstadoSilo    estado        = CARREGANDO;
bool          alarmeState   = false;
unsigned long ultimoBlink   = 0;
unsigned long ultimaLeitura = 0;

// ============================================================
void setup() {
  pinMode(PIN_VALVULA, OUTPUT);
  pinMode(PIN_ALARME,  OUTPUT);
  digitalWrite(PIN_VALVULA, HIGH); // inicia com válvula aberta
  digitalWrite(PIN_ALARME,  LOW);
  Serial.begin(9600);

  Serial.println("╔══════════════════════════════════════╗");
  Serial.println("║   Controle de Nível de Silo — v1.0   ║");
  Serial.println("╠══════════════════════════════════════╣");
  Serial.print  ("║  Reabertura   : 30%  → ADC "); Serial.print(NIVEL_REABERTURA);  Serial.println("        ║");
  Serial.print  ("║  Fechamento   : 80%  → ADC "); Serial.print(NIVEL_FECHAMENTO);  Serial.println("        ║");
  Serial.print  ("║  Transbordo   : 95%  → ADC "); Serial.print(NIVEL_TRANSBORDO);  Serial.println("        ║");
  Serial.println("╚══════════════════════════════════════╝\n");
}

// ============================================================
float adcParaPorcentagem(int adc) {
  return (adc / (float)ADC_MAX) * 100.0;
}

// ============================================================
String barraVisual(int adc) {
  int preenchido = map(adc, 0, ADC_MAX, 0, 20);
  String barra = "[";
  for (int i = 0; i < 20; i++) {
    if      (i < preenchido && preenchido >= 19) barra += "█"; // >95% — cheio crítico
    else if (i < preenchido && preenchido >= 16) barra += "▓"; // >80%
    else if (i < preenchido)                     barra += "░"; // normal
    else                                         barra += "·";
  }
  barra += "]";
  return barra;
}

// ============================================================
void abrirValvula() {
  digitalWrite(PIN_VALVULA, HIGH);
  Serial.println("  ► Válvula ABERTA");
}

void fecharValvula() {
  digitalWrite(PIN_VALVULA, LOW);
  Serial.println("  ► Válvula FECHADA");
}

void desligarAlarme() {
  digitalWrite(PIN_ALARME, LOW);
  alarmeState = false;
}

// ============================================================
void loop() {
  unsigned long agora = millis();

  // --- Piscar do alarme (independente do resto do loop) ---
  if (estado == TRANSBORDO) {
    if (agora - ultimoBlink >= BLINK_ALARME_MS) {
      ultimoBlink = agora;
      alarmeState = !alarmeState;
      digitalWrite(PIN_ALARME, alarmeState);
    }
  }

  // --- Leitura periódica do sensor ---
  if (agora - ultimaLeitura < LEITURA_MS) return;
  ultimaLeitura = agora;

  int   leitura    = analogRead(PIN_SENSOR);
  float porcentagem = adcParaPorcentagem(leitura);

  // ============================================================
  switch (estado) {

    // ----------------------------------------------------------
    case CARREGANDO:
      // Nível atingiu 95% → transbordo imediato
      if (leitura >= NIVEL_TRANSBORDO) {
        fecharValvula();
        estado = TRANSBORDO;
        Serial.println("\n⚠ TRANSBORDO DETECTADO — Alarme acionado!\n");

      // Nível atingiu 80% → fecha válvula, aguarda esvaziamento
      } else if (leitura >= NIVEL_FECHAMENTO) {
        fecharValvula();
        estado = CHEIO;
        Serial.println("\n  Nível máximo atingido — válvula fechada\n");
      }
      break;

    // ----------------------------------------------------------
    case CHEIO:
      // Nível subiu para 95% mesmo com válvula fechada → alarme
      if (leitura >= NIVEL_TRANSBORDO) {
        estado = TRANSBORDO;
        Serial.println("\n⚠ TRANSBORDO DETECTADO — Alarme acionado!\n");

      // Nível caiu abaixo de 30% → reabrir válvula
      } else if (leitura < NIVEL_REABERTURA) {
        abrirValvula();
        estado = CARREGANDO;
        Serial.println("\n  Nível baixo — válvula reaberta\n");
      }
      break;

    // ----------------------------------------------------------
    case TRANSBORDO:
      // Nível caiu abaixo de 30% → desativa alarme e reabre válvula
      if (leitura < NIVEL_REABERTURA) {
        fecharValvula(); // garante estado antes de reabrir
        desligarAlarme();
        abrirValvula();
        estado = CARREGANDO;
        Serial.println("\n  Nível seguro — alarme desativado, válvula reaberta\n");
      }
      break;
  }

  // --- Log serial ---
  Serial.print("NÍVEL: ");
  Serial.print(porcentagem, 1);
  Serial.print("%  ADC: ");
  Serial.print(leitura);
  Serial.print("  ");
  Serial.print(barraVisual(leitura));
  Serial.print("  Estado: ");

  switch (estado) {
    case CARREGANDO:  Serial.println("CARREGANDO  [válvula ABERTA ]"); break;
    case CHEIO:       Serial.println("CHEIO       [válvula FECHADA]"); break;
    case TRANSBORDO:  Serial.println("TRANSBORDO  [ALARME ATIVO   ]"); break;
  }
}
