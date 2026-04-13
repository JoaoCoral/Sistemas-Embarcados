const int ldrPin    = A0;
const int potPin    = A1;
const int ledPWM    = 3;   // LED 1 (PWM)
const int botaoA    = 2;   // Botão A (galpão ocupado)

const int BRILHO_VAZIO     = 26;   // 10% de 255
const int NIVEL_ALVO       = 512;  // nível alvo da soma (ajustável)

void setup() {
  pinMode(ledPWM,  OUTPUT);
  pinMode(botaoA,  INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  bool ocupado  = (digitalRead(botaoA) == LOW); // Pull-up: LOW = pressionado
  int  luzNat   = analogRead(ldrPin);           // 0~1023 (mais luz = maior valor)
  int  potVal   = analogRead(potPin);           // reservado para ajuste do nível alvo

  // Nível alvo pode ser ajustado pelo potenciômetro (256 ~ 768)
  int nivelAlvo = map(potVal, 0, 1023, 256, 768);

  int brilho = 0;

  if (!ocupado) {
    // Galpão vazio → força 10%
    brilho = BRILHO_VAZIO;
  } else {
    // Compensação: quanto mais luz natural, menos LED
    // brilho = nivelAlvo - luzNatural (convertido para 0~255)
    int compensado = nivelAlvo - luzNat;
    compensado = constrain(compensado, 0, 1023);
    brilho = map(compensado, 0, 1023, 0, 255);
  }

  analogWrite(ledPWM, brilho);

  // Debug Serial
  Serial.print("Ocupado: ");
  Serial.print(ocupado ? "SIM" : "NAO");
  Serial.print(" | Luz natural: ");
  Serial.print(luzNat);
  Serial.print(" | Nivel alvo: ");
  Serial.print(nivelAlvo);
  Serial.print(" | Brilho LED: ");
  Serial.print(map(brilho, 0, 255, 0, 100));
  Serial.println("%");

  delay(100);
}
