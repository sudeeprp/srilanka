#include <Servo.h>

Servo shivaservo;
Servo hiranyaservo;
Servo slvservo;

enum STATE {
  WAITINGVARAHA,
  PLAYING
};

STATE currentState = WAITINGVARAHA;
int angle = 0;

void print_angle(int target_angle) {
  Serial.print("going to degrees: ");
  Serial.println(target_angle);
}

void angulate_to(int target_angle) {
  if (angle == target_angle) {
    return;
  }
  int increment = 1;
  if (target_angle < angle) {
    increment = -1;
  }
  for(angle += increment; angle != target_angle && angle >= 0 && angle <= 90; angle += increment) {
    print_angle(angle);
    shivaservo.write(angle);
    delay(30);
  }
  print_angle(angle);
  shivaservo.write(angle);
}

int varahaLifted() {
  int ir = analogRead(A6);
  Serial.print("IR ");
  Serial.print(ir);
  Serial.print(" state ");
  Serial.println(currentState);
  return currentState == WAITINGVARAHA && ir > 500;
}

void killHiranyaksha() {
  Serial.println("hiranya fall to 135 after huaah");
  hiranyaservo.write(135);
  delay(1000);
}

void wavesAndKalyani() {
  Serial.println("pending: waves");
  // TODO: pulsate waves for 5s
  delay(5000);
  Serial.println("kalyani to 90");
  for (int rise = 0; rise <=90; ++rise) {
    slvservo.write(rise);
    delay(50);
  }
}

void shivaInHimalaya() {
  Serial.println("shiva");
  angulate_to(90);
}

void reset() {
  Serial.println("reset");
  angulate_to(0);
  hiranyaservo.write(0);
  slvservo.write(0);
  currentState = WAITINGVARAHA;
}

int resetRequested() {
  int request = 0;
  if (Serial.available() > 0) {
    int incomingByte = Serial.read();
    if (incomingByte == 'R') {
      request = 1;
    }
  }
  return request;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Sweep protect, pin 9 shiva slow, hiranya 10 fast, slv 11 slow. IR sensor");
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  shivaservo.attach(9);
  shivaservo.write(0);
  hiranyaservo.attach(10);
  hiranyaservo.write(0);
  slvservo.attach(11);
  slvservo.write(0);
  pinMode(A6, INPUT);
}

void loop() {
  if (varahaLifted()) {
    currentState = PLAYING;
    Serial.println("varaha");
    delay(4000);
    killHiranyaksha();
    wavesAndKalyani();
    shivaInHimalaya();
  } else if (resetRequested()) {
    reset();
  }
  delay(250);
}
