#include <Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial BT(12,13);
#define servoPin 11
Servo myServo;

#define RF_IN1 10
#define RB_IN2 9
#define LF_IN3 8
#define LB_IN4 7

#define ledLeft A1
#define ledRight A2

#define trig 3//Ultrasonic Sensor 
#define echo 2

#define buzzer A3

const int distancelimit= 20; 
char income; 

void setup() {
    Serial.begin(9600);
    BT.begin(9600); 

    pinMode(RF_IN1, OUTPUT); pinMode(RB_IN2, OUTPUT);
    pinMode(LF_IN3, OUTPUT); pinMode(LB_IN4, OUTPUT);

    pinMode(ledLeft, OUTPUT);
    pinMode(ledRight, OUTPUT);

    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(buzzer, OUTPUT);

    myServo.attach(servoPin);
    myServo.write(90); 
}

void loop() {
  Obstacle();
   if (Obstacle()) {
        stopMotors(); 
      }
  if (BT.available()) {
    income = BT.read();
    if (income == 'F') {
      myServo.write(90); 
      delay(200);
      if (Obstacle()) {
        stopMotors(); 
      }
      else {
        moveForward();
      }
    }
    else if (income == 'L') {
      myServo.write(10); 
      delay(200);
      if (Obstacle()) {
        stopMotors(); 
      }
      else {
        turnLeft();
      }
    }
    else if (income == 'R') {
      myServo.write(170); 
      delay(200);
      if (Obstacle()) {
        stopMotors(); 
      }
      else {
        turnRight();
      }
    }
    else {
      controlCar(income); 
    }
  }
}

bool Obstacle() {
  float duration;
  float distance;

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);

  return (distance > 0 && distance <= distancelimit);
}


void controlCar(char cmd) {
    switch (cmd) {
    case 'B': moveBackward(); break;
    case 'S': stopMotors(); break;
    default: stopMotors(); break; 
    }
}

void bothOn(){
  digitalWrite(ledLeft, HIGH); 
  digitalWrite(ledRight, HIGH);
}

void bothOff(){
  digitalWrite(ledLeft, LOW); 
  digitalWrite(ledRight,LOW );
}

void leftLed(){
  digitalWrite(ledLeft, HIGH); 
  digitalWrite(ledRight,LOW );
}

void rightLed(){
  digitalWrite(ledLeft, LOW); 
  digitalWrite(ledRight,HIGH );
}


void moveForward() {
    bothOn();
    digitalWrite(buzzer, LOW);

    digitalWrite(RF_IN1, HIGH); 
    digitalWrite(RB_IN2, LOW);
    digitalWrite(LF_IN3,HIGH); 
    digitalWrite(LB_IN4, LOW);
}

void moveBackward() {
    bothOff();
    digitalWrite(buzzer, HIGH);
    myServo.write(90); 

    digitalWrite(RF_IN1, LOW);
    digitalWrite(RB_IN2, HIGH); 
    digitalWrite(LF_IN3, LOW);
    digitalWrite(LB_IN4, HIGH); 
}

void turnLeft() {
    leftLed();
    digitalWrite(buzzer, LOW);

    digitalWrite(RF_IN1, HIGH);
    digitalWrite(RB_IN2, LOW);  // Right forward
    digitalWrite(LF_IN3, LOW);
    digitalWrite(LB_IN4, HIGH); // Left backward
}

void turnRight() {
    rightLed();
    digitalWrite(buzzer, LOW);

    digitalWrite(RF_IN1, LOW);
    digitalWrite(RB_IN2, HIGH); // Right backward
    digitalWrite(LF_IN3, HIGH);
    digitalWrite(LB_IN4, LOW);  // Left forward
}

void stopMotors() {
    bothOff();
    digitalWrite(buzzer, LOW);

    digitalWrite(RF_IN1, LOW);
    digitalWrite(RB_IN2, LOW);
    digitalWrite(LF_IN3, LOW);
    digitalWrite(LB_IN4, LOW);
}