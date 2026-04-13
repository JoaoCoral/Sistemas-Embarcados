// ============================================================
//  Indicador de Sentido PWM
//  - Potenciômetro no pino A0
//  - LED 1 no pino PWM 6  → brilha quando pot < 512
//  - LED 2 no pino PWM 9 → brilha quando pot > 512
//  - Zona morta central: 502–522 (ambos apagados)
// ============================================================

const int PIN_POT  = A0;
const int PIN_LED1 = 6;   // PWM
const int PIN_LED2 = 9;  // PWM

const int ZERO_CENTRAL = 512;
const int ZONA_MORTA   = 10;  // ±10 ao redor do zero → LEDs apagados
const int ADC_MAX      = 1023;

// ============================================================
void setup() {
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  analogWrite(PIN_LED1, 0);
  analogWrite(PIN_LED2, 0);
  Serial.begin(9600);
  Serial.println("Sistema pronto — gire o potenciômetro");
}

// ============================================================
void loop() {
  int leitura = analogRead(PIN_POT);

  int brilhoLed1 = 0;
  int brilhoLed2 = 0;

  if (leitura < ZERO_CENTRAL - ZONA_MORTA) {
    // --- Lado esquerdo: quanto mais longe de 512, mais brilho no LED 1 ---
    // leitura vai de (512-10)=502 até 0
    // brilho vai de 0 até 255
    brilhoLed1 = map(leitura, ZERO_CENTRAL - ZONA_MORTA, 0, 0, 255);
    brilhoLed1 = constrain(brilhoLed1, 0, 255);

  } else if (leitura > ZERO_CENTRAL + ZONA_MORTA) {
    // --- Lado direito: quanto mais longe de 512, mais brilho no LED 2 ---
    // leitura vai de (512+10)=522 até 1023
    // brilho vai de 0 até 255
    brilhoLed2 = map(leitura, ZERO_CENTRAL + ZONA_MORTA, ADC_MAX, 0, 255);
    brilhoLed2 = constrain(brilhoLed2, 0, 255);
  }
  // else: zona morta — ambos ficam em 0

  analogWrite(PIN_LED1, brilhoLed1);
  analogWrite(PIN_LED2, brilhoLed2);

  // --- Serial: barra visual de depuração ---
  Serial.print("POT: ");
  Serial.print(leitura);
  Serial.print(" | LED1: ");
  Serial.print(brilhoLed1);
  Serial.print(" | LED2: ");
  Serial.print(brilhoLed2);
  Serial.print("  [");

  // Barra visual: 20 chars, centro = posição 10
  int pos = map(leitura, 0, ADC_MAX, 0, 20);
  for (int i = 0; i <= 20; i++) {
    if (i == 10)       Serial.print("|");  // zero central
    else if (i == pos) Serial.print("█");
    else               Serial.print(".");
  }

  Serial.println("]");

  delay(40); // ~25 leituras/segundo, suave e sem flood no Serial
}
