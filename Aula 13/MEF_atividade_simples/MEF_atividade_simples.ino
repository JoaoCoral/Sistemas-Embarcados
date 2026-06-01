const int pin_botao_start = 2;
const int pin_botao_stop = 3;

const int sensor3 = 4;
const int sensor4 = 5;

const int motor_AH = 6;
const int motor_H = 7;

const int lampada = 8;

const int sensor1 = 9;
const int sensor2 = 10;


int estado = 0;

unsigned long tempo_anterior = 0, tempo_atual = 0;

int CA = 0;
int CB = 0;

void setup() {
  // put your setup code here, to run once:
 pinMode (pin_botao_start, INPUT_PULLUP);
 pinMode (pin_botao_stop, INPUT_PULLUP);

 pinMode (sensor1, INPUT);
 pinMode (sensor2, INPUT);

 pinMode (sensor3, INPUT);
 pinMode (sensor4, INPUT);

 pinMode (lampada, OUTPUT);

 pinMode (motor_H, OUTPUT);
 pinMode (motor_AH, OUTPUT);

 Serial.begin (9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  bool stop = digitalRead (pin_botao_stop);

  bool S1 = digitalRead (sensor1);
  bool S2 = digitalRead (sensor2);

  bool S3 = digitalRead (sensor3);
  bool S4 = digitalRead (sensor4);

  bool liga_H = digitalRead (motor_H);
  bool liga_AH = digitalRead (motor_AH);



  if (stop == 0) {
    estado = 0;
  }
  
  
  switch (estado) {
    case 0:
     bool start = digitalRead (pin_botao_start);
     if (start == 0) {
      estado = 1;
      tempo_anterior = millis();
      digitalWrite (lampada, HIGH);
      }
      break;

    case 1:
    tempo_atual = millis();
     if (tempo_atual - tempo_anterior > 2000) {
        estado = 2;
      }
      break;

    case 2:
     if (S3 == 1 && S4 == 1) {
      estado = 3;
     }
     if (S3 == 1 && S4 == 0) {
      estado = 4;
      }
      break;

    case 3:
      liga_AH = 1;
    if (S1 == 1) {
      estado = 5;
      }
      break;

    case 4:
      liga_H = 1;
    if (S2 == 1) {
      estado = 6;
      } 
      break;

    case 5:
     liga_AH = 0;
     CA++;
     estado = 1;
     tempo_anterior = millis();
     break;

    case 6:
     liga_H = 0;
     CB++;
     estado = 1;
     tempo_anterior = millis();
     break;















  }







}
