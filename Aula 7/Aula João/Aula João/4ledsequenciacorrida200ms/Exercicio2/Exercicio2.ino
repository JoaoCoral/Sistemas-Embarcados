#define LED1 7
#define LED2 8
#define LED3 9
#define LED4 10

int tempoAtual = 0, tempoAnterior = 0;
const int intervalo = 200;
int controleLed = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  tempoAtual = millis(); // Obtém o tempo atual

  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual; // Salva o último momento em que o LED piscou
    Serial.print("Tempo Atual: ");
    Serial.println(tempoAtual);
    Serial.print("Tempo Anterior: ");
    Serial.pritln(tempoAnterior);
    controleLed = controleLed+1;
    if (controleLed > 4)
      controleLed = 0;
    // Inverte o estado do LED
  }

  switch (controleLed){
    case 0:
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,LOW);
      digitalWrite(LED3,LOW);
      digitalWrite(LED4,LOW);
      break;
    case 1:
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,LOW);
      digitalWrite(LED3,LOW);
      digitalWrite(LED4,LOW);
      break;
    case 2:
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,HIGH);
      digitalWrite(LED3,LOW);
      digitalWrite(LED4,LOW);
      break;
    case 3:
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,HIGH);
      digitalWrite(LED3,HIGH);
      digitalWrite(LED4,LOW);
      break;
    case 4:
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,HIGH);
      digitalWrite(LED3,HIGH);
      digitalWrite(LED4,HIGH);
      break;
  }

}
