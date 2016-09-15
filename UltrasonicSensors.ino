
#include "avr/wdt.h"

#define NOT_AN_INTERRUPT -1

const int TRIG_PIN0 = 4;
const int TRIG_PIN1 = 5;
const int TRIG_PIN2 = 6;
const int TRIG_PIN3 = 8;
const int TRIG_PIN4 = 9;
const int TRIG_PINS[5] = {TRIG_PIN0, TRIG_PIN1, TRIG_PIN2, TRIG_PIN3, TRIG_PIN4};

//Interrupt Pins on Leonardo
const int ECHO_PIN0 = 0;
const int ECHO_PIN1 = 1;
const int ECHO_PIN2 = 2;
const int ECHO_PIN3 = 3;
const int ECHO_PIN4 = 7;

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

volatile boolean sensorsReading[5];

volatile unsigned long sensorStartTimes[5];
volatile unsigned long sensorEndTimes[5];

float sensorDistances[5];

void setup(){
  wdt_disable();
  Serial.begin(9600);
  
  for(int i=0; i<5; i++){
    sensorDistances[i] = 0;
    sensorStartTimes[i] = 0;
    sensorEndTimes[i] = 0;
    sensorsReading[i] = false;
  }
  
  pinMode(TRIG_PIN0, OUTPUT);
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(TRIG_PIN3, OUTPUT);
  pinMode(TRIG_PIN4, OUTPUT);
  
  digitalWrite(TRIG_PIN0, LOW);
  digitalWrite(TRIG_PIN1, LOW);
  digitalWrite(TRIG_PIN2, LOW);
  digitalWrite(TRIG_PIN3, LOW);
  digitalWrite(TRIG_PIN4, LOW);
  
  pinMode(ECHO_PIN0, INPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(ECHO_PIN3, INPUT);
  pinMode(ECHO_PIN4, INPUT);

  attachAllInterrupts();
  
  wdt_reset();
  wdt_enable(WDTO_1S);
}

void loop(){
  for(int i=0; i<5; i++){
    digitalWrite(TRIG_PINS[i], HIGH);
  }
  delayMicroseconds(10);
  for(int i=0; i<5; i++){
    digitalWrite(TRIG_PINS[i], LOW);
  }
  boolean working = true;
  for(int i=0; i<5; i++){
    if(!sensorsReading[i]){
      unsigned long pulse_width = sensorEndTimes[i] - sensorStartTimes[i];
      if( pulse_width > MAX_DIST){
        //Serial.print("Out of range: ");
        sensorDistances[i] = 157.48;
      }
      else{
        sensorDistances[i] = pulse_width/148.0;
      }
    }else{
      working = false;
      Serial.print("Sensor Reading: ");
      Serial.println(i);
    }
  }
  if(working){
    wdt_reset();
  }
  for(int i=0; i<5; i++){
    Serial.print("[");
    Serial.print(i);
    Serial.print(", ");
    Serial.print(sensorDistances[i]);
    Serial.print("]");
  }
  Serial.println("");
  delay(50);
}

void attachAllInterrupts(){
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN0), echoChange0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN1), echoChange1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN2), echoChange2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN3), echoChange3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN4), echoChange4, CHANGE);
}
void echoChange0(){
  if(!sensorsReading[0]){
    sensorsReading[0] = true;
    sensorStartTimes[0] = micros();
  }else{
    sensorsReading[0] = false;
    sensorEndTimes[0] = micros();
  }
}

void echoChange1(){
  if(!sensorsReading[1]){
    sensorsReading[1] = true;
    sensorStartTimes[1] = micros();
  }else{
    sensorsReading[1] = false;
    sensorEndTimes[1] = micros();
  }
}

void echoChange2(){
  if(!sensorsReading[2]){
    sensorsReading[2] = true;
    sensorStartTimes[2] = micros();
  }else{
    sensorsReading[2] = false;
    sensorEndTimes[2] = micros();
  }
}

void echoChange3(){
  if(!sensorsReading[3]){
    sensorsReading[3] = true;
    sensorStartTimes[3] = micros();
  }else{
    sensorsReading[3] = false;
    sensorEndTimes[3] = micros();
  }
}

void echoChange4(){
  if(!sensorsReading[4]){
    sensorsReading[4] = true;
    sensorStartTimes[4] = micros();
  }else{
    sensorsReading[4] = false;
    sensorEndTimes[4] = micros();
  }
}


