int botao = 2;




void setup() {
  Serial.begin (9600);
  pinMode(botao, INPUT_PULLUP);
  while (digitalRead(botao) == HIGH) {
    Serial.println ("Aguardando botão...");
    delay (500);
  }
  
 Serial.println ("Botão pressionado.");
  
  
}

void loop()
{

  
}
