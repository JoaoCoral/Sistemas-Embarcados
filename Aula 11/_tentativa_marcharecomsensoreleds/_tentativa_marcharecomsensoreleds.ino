#include <Ultrasonic.h>

Ultrasonic ultrasonic(12, 13);
int distance;

const int interruptpin = 3;
const int led1 = 7;
const int led2 = 8;
const int led3 = 9;
const int led4 = 10;
bool state1 = LOW;
bool state2 = LOW;
bool state3 = LOW;
bool state4 = LOW;


void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode (interruptpin, INPUT);
  pinMode (led1, OUTPUT);
  pinMode (led2, OUTPUT);
  pinMode (led3, OUTPUT);
  pinMode (led4, OUTPUT);
  attachInterrupt (interruptpin, marchare, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
 digitalWrite (led1,state1);
 digitalWrite (led2,state2);
 digitalWrite (led3,state3);
 digitalWrite (led4,state4);

}


void marchare () {

distance = ultrasonic.read();
  
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  delay(1000);
  
  if (distance <= 40) {
     state1 = 0;
     state2 = 0;    
     state3 = 0;
     state4 = 0;
  }

 else if (distance < 30) {
     state1 = 0;
     state2 = 0;    
     state3 = 0;
     state4 = 1;
  }
  else if (distance < 20) {
     state1 = 0;
     state2 = 0;    
     state3 = 1;
     state4 = 1;
  }

  else if (distance < 10) {
     state1 = 0;
     state2 = 1;    
     state3 = 1;
     state4 = 1;
  }

    else {
     state1 = 1;
     state2 = 1;    
     state3 = 1;
     state4 = 1;
  }



}


