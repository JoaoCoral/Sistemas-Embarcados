const int botaoStart  = 2;   // Inicia/reinicia o ciclo
const int ledAtuador  = 4;   // LED principal (atuador)
const int ledAndamento = 5;  // LED indicador de ciclo em andamento
const int ledPronto   = 6;   // LED indicador de sistema pronto

// --- Tempo do ciclo (ms) ---
const unsigned long TEMPO_CICLO = 5000; // 5 segundos

// --- Maquina de estados ---
#define ESTADO_PRONTO    0
#define ESTADO_OPERANDO  1
#define ESTADO_CONCLUIDO 2

int estado = ESTADO_PRONTO;

unsigned long tempoInicio = 0;
unsigned long tempoDecorrido = 0;

// --- Debounce ---
const unsigned long DEBOUNCE_MS = 200;
unsigned long ultimoBotao = 0;
bool estadoBotao = false;

void desligarTodos() {
  digitalWrite(ledAtuador,   LOW);
  digitalWrite(ledAndamento, LOW);
  digitalWrite(ledPronto,    LOW);
}

void entrarEmPronto() {
  estado = ESTADO_PRONTO;
  desligarTodos();
  digitalWrite(ledPronto, HIGH);
  Serial.println(">> Sistema pronto. Pressione o botao para iniciar.");
}

void iniciarCiclo() {
  estado      = ESTADO_OPERANDO;
  tempoInicio = millis();
  desligarTodos();
  digitalWrite(ledAtuador,   HIGH);
  digitalWrite(ledAndamento, HIGH);
  Serial.println(">> Ciclo iniciado!");
  Serial.print("   Duracao: ");
  Serial.print(TEMPO_CICLO / 1000);
  Serial.println(" segundos.");
}

void setup() {
  pinMode(botaoStart,   INPUT_PULLUP);
  pinMode(ledAtuador,   OUTPUT);
  pinMode(ledAndamento, OUTPUT);
  pinMode(ledPronto,    OUTPUT);

  Serial.begin(9600);
  Serial.println("=== Temporizador de Processo Industrial ===");
  entrarEmPronto();
}

void loop() {
  bool leitura  = (digitalRead(botaoStart) == LOW);
  unsigned long agora = millis();

  // --- Debounce com borda de subida ---
  if (leitura && !estadoBotao && (agora - ultimoBotao >= DEBOUNCE_MS)) {
    estadoBotao  = true;
    ultimoBotao  = agora;

    // Botao aceito em qualquer estado: reinicia o ciclo
    Serial.println("-- Botao pressionado.");
    iniciarCiclo();
  }
  if (!leitura) estadoBotao = false;

  // --- Logica do temporizador ---
  if (estado == ESTADO_OPERANDO) {
    tempoDecorrido = agora - tempoInicio;

    // Progresso a cada segundo
    unsigned long segundoAtual = tempoDecorrido / 1000;
    static unsigned long ultimoSegundo = 0;

    if (segundoAtual != ultimoSegundo) {
      ultimoSegundo = segundoAtual;
      unsigned long restante = (TEMPO_CICLO / 1000) - segundoAtual;
      Serial.print("   Tempo restante: ");
      Serial.print(restante);
      Serial.println("s");
    }

    // Fim do ciclo
    if (tempoDecorrido >= TEMPO_CICLO) {
      estado = ESTADO_CONCLUIDO;
      desligarTodos();
      Serial.println(">> Ciclo concluido!");
      Serial.println("   Pressione o botao para reiniciar.");
      delay(2000); // pausa visual antes de ir para pronto
      entrarEmPronto();
    }
  }

  delay(10);
}
