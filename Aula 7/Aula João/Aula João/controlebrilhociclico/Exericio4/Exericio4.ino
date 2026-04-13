#define botaoPulso 5
#define saidaLed   9

const long int intervalo = 200;
int tempoAtual = 0, tempoAnterior = 0;
int controleLed = 0;
long int saidaAnalogica = 0;
int tempoFiltroBotao = 0;
bool auxPulso = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(botaoPulso, INPUT_PULLUP);
  pinMode(saidaLed, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 tempoAtual = millis(); // Obtém o tempo atual

  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual; // Salva o último momento em que o LED piscou
    tempoFiltroBotao = 1;       // Tempo de 200ms de intervalo para o botão
  }

  if ((!digitalRead(botaoPulso)) && (!auxPulso) && (tempoFiltroBotao)){  //se pressionar o botão, ainda não foi pressionado e não deu o tempo
    controleLed = controleLed+25; //incrementa o controle do led    
    if (controleLed>100){
      controleLed = 0;
    }                     
    auxPulso = 1;  //intretavamento do pulso do botão
  }
  else if (digitalRead(botaoPulso)){  //se soltar o botão fica zerando as variáveis
    auxPulso = 0;
    tempoFiltroBotao = 0;
  }

  saidaAnalogica = (controleLed*255)/100;
  analogWrite(saidaLed, saidaAnalogica);

 /* Serial.print("Controle LED: ");
  Serial.println(controleLed);
  Serial.print("Aux Pulso: ");
  Serial.println(auxPulso);*/
}
