const int botaoA   = 2;
const int botaoB   = 3;
const int ledVerde  = 4;  // Maquina acionada
const int ledAmarelo = 5; // Um botao pressionado (aguardando)
const int ledVermelho = 6; // Falha de simultaneidade

const unsigned long JANELA_MS = 200; // janela de simultaneidade (ms)

unsigned long tempoA = 0; // momento em que botao A foi pressionado
unsigned long tempoB = 0; // momento em que botao B foi pressionado

bool estadoA = false;
bool estadoB = false;
bool maquinaAtiva = false;

void desligarTodos() {
  digitalWrite(ledVerde,    LOW);
  digitalWrite(ledAmarelo,  LOW);
  digitalWrite(ledVermelho, LOW);
}

void setup() {
  pinMode(botaoA,     INPUT_PULLUP);
  pinMode(botaoB,     INPUT_PULLUP);
  pinMode(ledVerde,   OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho,OUTPUT);
  desligarTodos();
  Serial.begin(9600);
  Serial.println("=== Sistema Bimanual Pronto ===");
  Serial.println("Pressione A e B simultaneamente para acionar.");
}

void loop() {
  bool leituraA = (digitalRead(botaoA) == LOW);
  bool leituraB = (digitalRead(botaoB) == LOW);
  unsigned long agora = millis();

  // --- Detecta borda de subida (botao acabou de ser pressionado) ---
  if (leituraA && !estadoA) {
    estadoA = true;
    tempoA  = agora;
    Serial.println("Botao A pressionado.");
  }
  if (leituraB && !estadoB) {
    estadoB = true;
    tempoB  = agora;
    Serial.println("Botao B pressionado.");
  }

  // --- Detecta borda de descida (botao solto) ---
  if (!leituraA && estadoA) {
    estadoA = false;
    Serial.println("Botao A solto.");
  }
  if (!leituraB && estadoB) {
    estadoB = false;
    Serial.println("Botao B solto.");
  }

  // --- Logica de simultaneidade ---
  desligarTodos();
  maquinaAtiva = false;

  if (estadoA && estadoB) {
    // Ambos pressionados: verifica janela de tempo
    unsigned long diferenca = abs((long)tempoA - (long)tempoB);

    if (diferenca <= JANELA_MS) {
      // Simultaneidade valida: aciona maquina
      maquinaAtiva = true;
      digitalWrite(ledVerde, HIGH);
      Serial.println(">> MAQUINA ACIONADA (simultaneidade OK)");

    } else {
      // Ambos pressionados mas fora da janela
      digitalWrite(ledVermelho, HIGH);
      Serial.print(">> FALHA: diferenca de ");
      Serial.print(diferenca);
      Serial.println("ms - fora da janela!");
    }

  } else if (estadoA || estadoB) {
    // Apenas um pressionado: aguardando o outro
    digitalWrite(ledAmarelo, HIGH);

    // Verifica se passou a janela sem o segundo botao
    unsigned long tempoUnico = estadoA ? tempoA : tempoB;
    if (agora - tempoUnico > JANELA_MS) {
      // Janela expirou com apenas um botao
      digitalWrite(ledAmarelo, LOW);
      digitalWrite(ledVermelho, HIGH);
      Serial.println(">> FALHA: janela expirada com apenas um botao.");
    }
  }

  delay(10); // debounce leve
}
