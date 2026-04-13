#define LED1 7
#define LED2 8
#define LED3 9
#define LED4 10

long int valorAnalogico = 0;  //variável que recebe o valor da entrada analógica
long int valorPorcentagem = 0; //variável para converter em porcentagem

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
  valorAnalogico = analogRead(A0);
  //valorPorcentagem = map(valorAnalogico, 0, 1023, 0, 100);  //converte o valor com map
  valorPorcentagem = (valorAnalogico*100)/1023;  //converte em valor por regra de 3
  Serial.print("Valor Analogico: ");
  Serial.println(valorAnalogico);
  Serial.print("Valor Porcentagem: ");
  Serial.println(valorPorcentagem);
 
  if (valorPorcentagem<=25){  //valor da analógica menor que 25% desliga tudo
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    digitalWrite(LED4,LOW);
  }
  else if ((valorPorcentagem>=25) && (valorPorcentagem<=50)){  //entre 25 e 50% liga led 1
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    digitalWrite(LED4,LOW);   
  }
  else if ((valorPorcentagem>=50) && (valorPorcentagem<=75)){ //entre 50 e 75% liga led 1 e 2
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED3,LOW);
    digitalWrite(LED4,LOW);   
  }
  else if ((valorPorcentagem>=75) && (valorPorcentagem<=90)){ //entre 75 e 90% liga led 1, 2 e 3
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED3,HIGH);
    digitalWrite(LED4,LOW);   
  }
  else{ //acima de 90 liga todos
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED3,HIGH);
    digitalWrite(LED4,HIGH);    
  }

}
