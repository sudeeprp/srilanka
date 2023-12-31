#include <Servo.h>

Servo shivaservo;
Servo hiranyaservo;
Servo slvservo;
const int pointerPin = 3;

enum STATE {
  WAITINGVARAHA,
  PLAYING
};

STATE currentState = WAITINGVARAHA;
int angle = 0;
const int max_angle = 110;

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
  for(angle += increment; angle != target_angle && angle >= 0 && angle <= max_angle; angle += increment) {
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
  // TODO: pulsate waves
  delay(4000);
  // varaha + (6s)
  Serial.println("kalyani to 90");
  for (int rise = 0; rise <=90; ++rise) {
    slvservo.write(rise);
    delay(50);
  }
  // varaha + 6s + (50*90=4.5s)
  delay(6500);
  // varaha + 6s + 4.5s + (6.5s)
}

void shivaInHimalaya() {
  Serial.println("shiva");
  angulate_to(110);
  // shiva + (30*110=3.3s)
  delay(23400);
  // shiva + 3.3s + (23.4s)
}

void pulsateTandava() {
  for(int i = 0; i < 5; i++) {
    shivaservo.write(max_angle + 3);
    delay(100);
    shivaservo.write(max_angle);
    delay(1700);
  }
  // tandava + (1.8*5=9s)
  for(int j = 0; j < 15; j++) {
    shivaservo.write(max_angle + 3);
    delay(100);
    shivaservo.write(max_angle);
    delay(300);    
  }
  //tandava + 9s + (0.4*15=6s)
  delay(4000);
  //tandava + 9s + 6s + (4s)
}

void reset() {
  Serial.println("reset");
  angulate_to(0);
  hiranyaservo.write(0);
  slvservo.write(0);
  currentState = WAITINGVARAHA;
}

int serialInput() {
  int incomingByte = 0;
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
  }
  return incomingByte;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Sweep protect, pin 9 shiva 110, hiranya 10 fast, slv 11 slow. IR sensor");
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(pointerPin, OUTPUT);
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
    // huaah+4 s
    killHiranyaksha();
    wavesAndKalyani();
    shivaInHimalaya();
    pulsateTandava();
  } else {
    int incoming = serialInput();
    if (incoming == 'R') {
      reset();
    } else if (incoming == 'P') {
      Serial.println("pointer on");
      digitalWrite(pointerPin, HIGH);
    } else if (incoming == 'Q') {
      Serial.println("pointer off");
      digitalWrite(pointerPin, LOW);
    }
  }
  delay(250);
}
