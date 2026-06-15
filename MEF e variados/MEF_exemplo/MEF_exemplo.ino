const int pin_botao_start = 2;
const int pin_botao_stop = 3;
int estado = 0;
unsigned long tempo_ant = 0;  
unsigned long tempo_atual = 0;
const int motorAH = 4;
const int motorH = 5;
int s1 = 0;
int s2 = 0;
int s3 = 0;
int s4 =0;
int CA = 0;
int CB = 0;

void setup() {
  pinMode(pin_botao_start, INPUT_PULLUP);
  pinMode(pin_botao_stop, INPUT_PULLUP);

  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(s4, OUTPUT);

}

void loop() {
  bool sensor1 = digitalRead(s1);
  bool sensor2 = digitalRead(s2);
  bool sensor3 = digitalRead(s3);
  bool sensor4 = digitalRead(s4);
  bool stop = digitalRead(pin_botao_stop);
  if (stop ==0){
    estado = 0;
  }

  switch (estado){
    case 0:
      bool start = digitalRead(pin_botao_start);
      if (start ==0){
        estado = 1;
        tempo_ant = millis();
      }
      break;
    case 1:
      tempo_atual = millis();
      if(tempo_atual - tempo_ant > 2000){
        estado = 2;
      }
      break;
    case 2:
      if(s3 == 1 && s4 ==1){
        estado =3 ;
      }
      else if(s3  == 1 && s4 == 0){
        estado = 4;
      }
      break;
    case 3:
      digitalWrite(motorAH, HIGH);
      if(sensor1 == 1){
        estado = 5;
      }
      break;
    case 4:
      digitalWrite(motorH, HIGH);
      if(sensor2 == 1){
        estado =  6;
      }
      break;
    case 5:
      digitalWrite(motorAH, LOW);
      CA++;
        tempo_ant = millis();
        estado = 1;
      break;
    case 6:
      digitalWrite(motorH, LOW);
      CB++ ;
      estado = 1;
    break;

        
  }
  if(pin_botao_stop == 1){
    digitalWrite(motorAH, LOW);
    digitalWrite(motorH, LOW);
  }
  }
