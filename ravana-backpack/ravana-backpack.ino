int PWMA=5;//Right side 
int PWMB=4;//Left side 
int DA=0;//Right reverse 
int DB=2;//Left reverse 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("dummy off on real light");
  pinMode(PWMA, OUTPUT); 
  pinMode(DA, OUTPUT);
}

void loop() {
  Serial.print("Analog input: ");
  int photoVoltaic = analogRead(A0);
  Serial.println(photoVoltaic);
  if (photoVoltaic > 90) {
    Serial.println("off the solenoid!");
    digitalWrite(DA, LOW);
    digitalWrite(PWMA, LOW);
  } else {
    Serial.println("hold");
    digitalWrite(DA, HIGH);
    digitalWrite(PWMA, HIGH);
  }
  delay(1000);
}
