const int potPin      = A0;
const int ledVerde    = 2;
const int ledAmarelo  = 3;
const int ledVermelho = 4;
const int ledChave    = 5;

const float V_MIN_NORMAL  = 210.0;
const float V_MAX_NORMAL  = 230.0;
const float V_MIN_SEGURO  = 180.0;
const float V_MAX_SEGURO  = 240.0;
const unsigned long TEMPO_DESLIGA = 2000;

unsigned long tempoForaDaFaixa = 0;
bool foraAntes  = false;
bool chaveAtiva = true;

void setup() {
  pinMode(ledVerde,    OUTPUT);
  pinMode(ledAmarelo,  OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledChave,    OUTPUT);
  digitalWrite(ledChave, HIGH);
  Serial.begin(9600);
}

void desligarTodosLEDs() {
  digitalWrite(ledVerde,    LOW);
  digitalWrite(ledAmarelo,  LOW);
  digitalWrite(ledVermelho, LOW);
}

void loop() {
  int leitura = analogRead(potPin);
  float tensao = (leitura / 1023.0) * 250.0;

  Serial.print("Tensão: ");
  Serial.print(tensao, 1);
  Serial.print(" V | Chave: ");
  Serial.println(chaveAtiva ? "LIGADA" : "DESLIGADA");

  // Apaga todos antes de decidir
  desligarTodosLEDs();

  // LEDs de status — condições exclusivas e explícitas
  if (tensao >= V_MIN_NORMAL && tensao <= V_MAX_NORMAL) {
    digitalWrite(ledVerde, HIGH);         // 210V ~ 230V → Normal
  } else if (tensao < V_MIN_NORMAL) {
    digitalWrite(ledAmarelo, HIGH);       // < 210V → Under Voltage
  } else if (tensao > V_MAX_NORMAL) {
    digitalWrite(ledVermelho, HIGH);      // > 230V → Over Voltage
  }

  // Proteção da chave geral (< 180V ou > 240V por mais de 2s)
  bool foraDaFaixa = (tensao < V_MIN_SEGURO || tensao > V_MAX_SEGURO);

  if (foraDaFaixa) {
    if (!foraAntes) {
      tempoForaDaFaixa = millis();
      foraAntes = true;
    } else if (chaveAtiva && (millis() - tempoForaDaFaixa >= TEMPO_DESLIGA)) {
      chaveAtiva = false;
      digitalWrite(ledChave, LOW);
      Serial.println("ALERTA: Chave geral desligada por segurança!");
    }
  } else {
    foraAntes = false;
    if (!chaveAtiva) {
      chaveAtiva = true;
      digitalWrite(ledChave, HIGH);
      Serial.println("Tensão normalizada. Chave reativada.");
    }
  }

  delay(100);
}
