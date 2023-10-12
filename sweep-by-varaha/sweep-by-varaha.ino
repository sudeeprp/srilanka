#include <Servo.h>

Servo myservo;
enum STATE {
  WAITINGVARAHA,
  PLAYING
};

STATE currentState = WAITINGVARAHA;
int angle = 0;

void print_angle(int target_angle) {
  Serial.print("was ms: ");
  Serial.print(myservo.readMicroseconds());
  Serial.print("; degrees: ");
  Serial.println(myservo.read());
  Serial.print("going to degrees: ");
  Serial.println(target_angle);
}

void angulate_to(int target_angle) {
  int increment = 1;
  if (target_angle < angle) {
    increment = -1;
  }
  for(angle += increment; angle != target_angle; angle += increment) {
    print_angle(angle);
    myservo.write(angle);
    delay(30);
  }
  print_angle(angle);
  myservo.write(angle);
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
  Serial.println("pending: hiranyaksha");
  delay(2000);
}

void wavesAndKalyani() {
  Serial.println("pending: waves & kalyani");
  delay(5000);
}

void shivaInHimalaya() {
  Serial.println("shiva");
  angulate_to(90);
}

void reset() {
  Serial.println("reset");
  angulate_to(0);
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
  Serial.println("Sweep 90, pin 9 slow. IR sensor");
  myservo.attach(9);
  pinMode(A6, INPUT);
}

void loop() {
  if (varahaLifted()) {
    currentState = PLAYING;
    Serial.println("varaha");
    delay(3000);
    killHiranyaksha();
    wavesAndKalyani();
    shivaInHimalaya();
  } else if (resetRequested()) {
    reset();
  }
  delay(250);
}
