#include <Servo.h>

Servo myservo;
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
    delay(500);
    print_laser();
  }
  print_angle(angle);
  myservo.write(angle);
}

void print_laser() {
  int laser = analogRead(A6);
  Serial.print("laser ");
  Serial.println(laser);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Sweep from pin 9 slow");
  myservo.attach(9);
  pinMode(A6, INPUT);
}

void loop() {
  Serial.println("down");
  angulate_to(17);
  delay(3000);
  Serial.println("up");
  angulate_to(0);
  delay(3000);
}
