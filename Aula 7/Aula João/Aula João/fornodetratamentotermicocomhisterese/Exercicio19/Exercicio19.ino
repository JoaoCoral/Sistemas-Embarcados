#define saidaAquecimento 9
#define saidaDentroFaixa 10

#define botaoLigaSistema 3
#define botaoDeslSistema 5

long int entradaAnalogica = 0;
long int temperaturaNTC = 0;
bool sistemaOn = 0;
bool subiuTemperatura = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(saidaAquecimento, OUPTUT);
  pinMode(saidaDentroFaixa, OUPTUT);
  pinMode(botaoLigaSistema, INPUT_PULLUP);
  pinMode(botaoDeslSistema, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  entradaAnalogica = analogRead(A0);
  temperaturaNTC = (entradaAnalogica*250)/1023;

  if (!digitalRead(botaoLigaSistema)){
    sistemaOn = 1;
  }
  if (!digitalRead(botaoDeslSistema)){
    sistemaOn = 0;
  }

  if (sitemaOn){
    if (temperaturaNTC<150){
      digitalWrite(saidaAquecimento, HIGH);
      digitalWrite(saidaDentroFaixa, LOW);
      subiuTemperatura = 0; 
    }
    else if ((temperaturaNTC>=150) && (temperaturaNTC<160) && (!subiuTemperatura)){
      digitalWrite(saidaAquecimento, HIGH);
      digitalWrite(saidaDentroFaixa, HIGH);  
       
    }
    else if (temperaturaNTC>=160){
      digitalWrite(saidaAquecimento, LOW);
      digitalWrite(saidaDentroFaixa, LOW);   
      subiuTemperatura = 1;    
    }

  }
  else{
    digitalWrite(saidaAquecimento, LOW);
    digitalWrite(saidaDentroFaixa, LOW);
  }



}
