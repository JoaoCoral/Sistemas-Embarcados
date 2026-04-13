#define botaoHab  4
#define botaoLiga 5

#define saidaA 10

void setup() {
  // put your setup code here, to run once:
  pinMode(botaoHab, INPUT_PULLUP);
  pinMode(botaoLiga, INPUT_PULLUP);

  pinMode(saidaA, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!digitalRead(botaoHab)){  //botão habilita ligado
    if (!digitalRead(botaoLiga)){ //botão liga 
      digitalWrite(saidaA, HIGH); //liga saída 
    }
    else{
      digitalWrite(saidaA, LOW); //desliga saída
    }
  }
  else{
    digitalWrite(saidaA, LOW);  //desliga saída
  }
}
