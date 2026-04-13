// ============================================================
//  Média Móvel Simples
//  - Potenciômetro no pino A0
//  - LED 3 no pino digital 6
//  - Lê 10 amostras com intervalo de 50ms
//  - Acende LED 3 se média > 700
// ============================================================

const int PIN_POT  = A0;
const int PIN_LED3 = 6;

const int   NUM_AMOSTRAS = 10;
const int   INTERVALO_MS = 50;
const int   LIMIAR       = 700;
const unsigned long CICLO_MS = 600; // intervalo entre ciclos de leitura

unsigned long ultimoCiclo = 0;

// ============================================================
void setup() {
  pinMode(PIN_LED3, OUTPUT);
  digitalWrite(PIN_LED3, LOW);
  Serial.begin(9600);
  Serial.println("Sistema pronto — ciclo de leitura a cada 600 ms");
}

// ============================================================
float coletarMedia() {
  long soma = 0;

  for (int i = 0; i < NUM_AMOSTRAS; i++) {
    int leitura = analogRead(PIN_POT);
    soma += leitura;

    Serial.print("  Amostra ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(leitura);

    delay(INTERVALO_MS);
  }

  return (float)soma / NUM_AMOSTRAS;
}

// ============================================================
void loop() {
  if (millis() - ultimoCiclo >= CICLO_MS) {
    ultimoCiclo = millis();

    Serial.println("\n--- Novo ciclo de leitura ---");

    float media = coletarMedia();

    Serial.print("Média: ");
    Serial.println(media, 1);

    if (media > LIMIAR) {
      digitalWrite(PIN_LED3, HIGH);
      Serial.println("Média > 700 — LED 3 ACESO");
    } else {
      digitalWrite(PIN_LED3, LOW);
      Serial.println("Média ≤ 700 — LED 3 apagado");
    }
  }
}
