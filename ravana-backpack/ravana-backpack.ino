int PWMA=5;//Right side 
int PWMB=4;//Left side 
int DA=0;//Right reverse 
int DB=2;//Left reverse 

#include "EspMQTTClient.h"

EspMQTTClient client(
  "Sharanagati",
  "omnamonarayanaya111",
  "mqtt.eclipseprojects.io",  // MQTT Broker server ip
  "backpack-289738473847"     // Client name that uniquely identify your device
);

void pollForLight() {
  Serial.print("Analog input: ");
  int photoVoltaic = analogRead(A0);
  Serial.println(photoVoltaic);
  if (photoVoltaic > 105) {
    Serial.println("off the solenoid!");
    client.publish("varaha/final", "D");
    digitalWrite(DA, LOW);
    digitalWrite(PWMA, LOW);
  } else {
    Serial.println("hold");
    digitalWrite(DA, HIGH);
    digitalWrite(PWMA, HIGH);
  }
//  char lightstr[16];
//  sprintf(lightstr, "%d", photoVoltaic);
//  client.publish("varaha/light", lightstr, true);
  client.executeDelayed(500, []() {
    pollForLight();
  });
}

void onConnectionEstablished() {
  Serial.println("Connected");
  client.executeDelayed(2500, []() {
    client.publish("varaha/final", "S");
  });
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("off on pointer, wifi");
  pinMode(PWMA, OUTPUT); 
  pinMode(DA, OUTPUT);
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
  pollForLight();
}

void loop() {
  client.loop();
}
