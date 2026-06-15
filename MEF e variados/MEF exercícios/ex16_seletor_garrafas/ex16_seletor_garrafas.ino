
const int pin_L  = 2;
const int pin_D  = 3; 
const int pin_sP = 4; 
const int pin_sM = 5;  
const int pin_sG = 6;  
const int pin_A  = 7;  
const int pin_B  = 8;  
const int pin_C  = 9;  

const int pin_E  = 10;
const int pin_AL = 11;

int estado = 0;
int tipo_selecionado = 0;
int tipo_detectado   = 0;

void setup() {
  pinMode(pin_L,  INPUT_PULLUP);
  pinMode(pin_D,  INPUT_PULLUP);
  pinMode(pin_sP, INPUT);
  pinMode(pin_sM, INPUT);
  pinMode(pin_sG, INPUT);
  pinMode(pin_A,  INPUT);
  pinMode(pin_B,  INPUT);
  pinMode(pin_C,  INPUT);

  pinMode(pin_E,  OUTPUT);
  pinMode(pin_AL, OUTPUT);

  Serial.begin(9600);
  digitalWrite(pin_E,  LOW);
  digitalWrite(pin_AL, LOW);
}

void loop() {
  bool btn_L = (digitalRead(pin_L) == LOW);
  bool btn_D = (digitalRead(pin_D) == LOW);
  bool selP  = digitalRead(pin_sP);
  bool selM  = digitalRead(pin_sM);
  bool selG  = digitalRead(pin_sG);
  bool senA  = digitalRead(pin_A);
  bool senB  = digitalRead(pin_B);
  bool senC  = digitalRead(pin_C);

  
  if (selP)      tipo_selecionado = 1;
  else if (selM) tipo_selecionado = 2;
  else if (selG) tipo_selecionado = 3;

  
  if (btn_D) {
    digitalWrite(pin_E,  LOW);
    digitalWrite(pin_AL, LOW);
    estado = 0;
    return;
  }

  switch (estado) {

    case 0: // aguarda botoeira L
      digitalWrite(pin_E,  LOW);
      digitalWrite(pin_AL, LOW);
      if (btn_L) {
        digitalWrite(pin_E, HIGH); // liga esteira
        Serial.print("Esteira ligada. Selecionado: ");
        Serial.println(tipo_selecionado == 1 ? "PEQUENA" :
                       tipo_selecionado == 2 ? "MEDIA"   : "GRANDE");
        estado = 1;
      }
      break;

    case 1:
      if (senA && !senB && !senC) {
        tipo_detectado = 1;
        estado = 2;
      } else if (senA && senB && !senC) {
        tipo_detectado = 2;
        estado = 2;
      } else if (senA && senB && senC) {
        tipo_detectado = 3;
        estado = 2;
      }
      break;

    case 2:
      if (tipo_detectado == tipo_selecionado) {
        Serial.println("Garrafa correta. Continuando.");
        estado = 1;
      } else {
        digitalWrite(pin_E,  LOW);
        digitalWrite(pin_AL, HIGH);
        Serial.println("ALARME: garrafa incorreta! Retirar manualmente.");
        estado = 3;
      }
      break;

    case 3: 
      if (!senA) {
        Serial.println("Garrafa retirada. Aguardando novo acionamento em L.");
        digitalWrite(pin_AL, LOW);
        estado = 0;
      }
      break;
  }
}
