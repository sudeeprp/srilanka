const int PWMA=5;//Right side 
const int PWMB=4;//Left side 
const int DA=0;//Right reverse 
const int DB=2;//Left reverse 
const int trigPin = 12;
const int echoPin = 14;

enum STATE {
  MOVINGUP,
  MOVINGDOWN,
  STATIONARY
};

int downLimitCm = 40;
int upLimitCm = 5;
STATE currentState = STATIONARY;
int downStartMillis = 0;
const int downwardTimeoutms= 15000;

void startMoveUp() {
  digitalWrite(DA, LOW);
  digitalWrite(DB, LOW);
  digitalWrite(PWMA, HIGH);
  digitalWrite(PWMB, HIGH);
  currentState = MOVINGUP;  
}

void startMoveDown() {
  digitalWrite(DA, HIGH);
  digitalWrite(DB, HIGH);
  digitalWrite(PWMA, HIGH);
  digitalWrite(PWMB, HIGH);
  currentState = MOVINGDOWN;
  downStartMillis = millis();
}

void stop() {
  digitalWrite(DA, LOW);
  digitalWrite(DB, LOW);
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  currentState = STATIONARY;
  downStartMillis = 0;
}

void monitorDistance() {
  if (currentState == STATIONARY) {
    stop();
  } else {
    const float SOUND_VELOCITY = 0.034;
  
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    float distanceCm = duration * SOUND_VELOCITY/2;
    Serial.print("distance (cm): ");
    Serial.println(distanceCm);
    if (currentState == MOVINGUP) {
      if (distanceCm <= upLimitCm) {
        stop();
      }
    } else if (currentState == MOVINGDOWN) {
      int downwardElapsedms = millis() - downStartMillis;
      Serial.print("downward ms: ");
      Serial.println(downwardElapsedms);
      if (distanceCm >= downLimitCm || downwardElapsedms > downwardTimeoutms) {
        stop();
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println('slider with dist');
  pinMode(PWMA, OUTPUT); 
  pinMode(DA, OUTPUT);
  
  pinMode(PWMB, OUTPUT);
  pinMode(DB, OUTPUT);
//  digitalWrite(PWMB, HIGH);
//  digitalWrite(DB, HIGH);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  if (Serial.available() > 0) {
    int incomingByte = Serial.read();

    switch(incomingByte) {
      case 'D':
        Serial.println("Down");
        startMoveDown();
        break;
      case 'U':
        Serial.println("Up");
        startMoveUp();
        break;
      case 'S':
        Serial.println("Stop");
        stop();
        break;
    }
  }
  monitorDistance();
  Serial.print("slider15.b ");
  Serial.println(currentState);
  delay(250);
}
