const int botaoA     = 2;
const int ledPressao = 4;   // LED 1 - Atuador de pressão
const int ledSolda   = 3;   // LED 2 - Corrente de solda (PWM)
const int ledPronto  = 5;   // LED 3 - Pronto
const int potPin     = A0;  // Potenciômetro - potência da solda

// Tempos do ciclo (ms)
const unsigned long T_ESTABILIZACAO = 800;
const unsigned long T_PRE_SOLDA     = 200;
const unsigned long T_SOLDA         = 1500;
const unsigned long T_RESFRIAMENTO  = 500;
const unsigned long T_PRONTO        = 2000;

bool cicloAtivo = false;

void desligarTudo() {
  digitalWrite(ledPressao, LOW);
  analogWrite(ledSolda,    0);
  digitalWrite(ledPronto,  LOW);
}

void executarCiclo(int potencia) {
  // --- 1. Estabilização mecânica ---
  Serial.println("[1/5] Aguardando estabilizacao mecanica...");
  delay(T_ESTABILIZACAO);

  // --- 2. Fecha eletrodo (pressão) ---
  Serial.println("[2/5] Eletrodo fechado - pressao aplicada.");
  digitalWrite(ledPressao, HIGH);
  delay(T_PRE_SOLDA);

  // --- 3. Corrente de solda ---
  Serial.print("[3/5] Corrente de solda ativa - Potencia: ");
  Serial.print(map(potencia, 0, 255, 0, 100));
  Serial.println("%");
  analogWrite(ledSolda, potencia);
  delay(T_SOLDA);

  // --- 4. Resfriamento sob pressão ---
  Serial.println("[4/5] Solda concluida - resfriando sob pressao...");
  analogWrite(ledSolda, 0);
  delay(T_RESFRIAMENTO);

  // --- 5. Libera peça ---
  Serial.println("[5/5] Peca liberada - ciclo completo!");
  digitalWrite(ledPressao, LOW);
  digitalWrite(ledPronto,  HIGH);
  delay(T_PRONTO);
  digitalWrite(ledPronto,  LOW);
}

void setup() {
  pinMode(botaoA,     INPUT_PULLUP);
  pinMode(ledPressao, OUTPUT);
  pinMode(ledSolda,   OUTPUT);
  pinMode(ledPronto,  OUTPUT);

  desligarTudo();
  Serial.begin(9600);
  Serial.println("=== Maquina de Solda Ponto Pronta ===");
  Serial.println("Pressione o Botao A para iniciar o ciclo.");
}

void loop() {
  // Aguarda botão (LOW = pressionado com INPUT_PULLUP)
  if (digitalRead(botaoA) == LOW && !cicloAtivo) {
    cicloAtivo = true;
    desligarTudo();

    // Lê potência do potenciômetro e converte para 0~255
    int potencia = map(analogRead(potPin), 0, 1023, 0, 255);

    Serial.println("\n--- Iniciando ciclo de solda ---");
    Serial.print("Potencia configurada: ");
    Serial.print(map(potencia, 0, 255, 0, 100));
    Serial.println("%");

    executarCiclo(potencia);

    Serial.println("--- Sistema aguardando novo ciclo ---\n");
    cicloAtivo = false;
  }

  delay(50); // debounce simples
}
