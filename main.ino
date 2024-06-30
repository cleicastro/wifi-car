// you can enable debug logging to Serial at 115200
// #define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Servo.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "BOT"
#define REMOTEXY_WIFI_PASSWORD "123456789"
#define REMOTEXY_SERVER_PORT 6377
#define REMOTEXY_ACCESS_PASSWORD "123456789"

#include <RemoteXY.h>

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 85 bytes
  { 255,5,0,0,0,78,0,17,0,0,0,27,2,200,84,106,200,1,1,5,
  0,3,122,28,70,26,19,48,66,24,131,2,26,1,48,9,24,24,36,83,
  33,33,0,2,31,0,1,68,34,24,24,66,116,32,32,0,2,31,0,1,
  47,58,24,24,36,147,34,34,0,2,31,0,1,27,34,24,24,6,113,33,
  33,0,2,31,0 };

// this structure defines all the variables and events of your control interface 
struct {
    uint8_t select_01; // from 0 to 3
    uint8_t button_01; // =1 if button pressed, else =0
    uint8_t button_02; // =1 if button pressed, else =0
    uint8_t button_03; // =1 if button pressed, else =0
    uint8_t button_04; // =1 if button pressed, else =0

    // other variable
    uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define motorPWMA D1
#define motorADirPin D3
#define motorPWMB D2
#define motorBDirPin D4

#define servoGPO D6
#define trigGPO D7
#define echoGPO D8

// #define led_power D7

Servo servo;

const unsigned long interval = 5000;
unsigned long lastMsg = 0;
int distance = 100;
int motor_speed = 1024;

void moveForward(int speed) {
  analogWrite(motorPWMA, speed);
  digitalWrite(motorADirPin, HIGH);

  analogWrite(motorPWMB, speed);
  digitalWrite(motorBDirPin, HIGH);
}

void moveBackward(int speed) {
  analogWrite(motorPWMA, speed);
  digitalWrite(motorADirPin, LOW);
  
  analogWrite(motorPWMB, speed);
  digitalWrite(motorBDirPin, LOW);
  Serial.println("backward");
}

void moveLeftForward(int speed) {
  analogWrite(motorPWMA, speed / 2);
  digitalWrite(motorADirPin, HIGH);

  analogWrite(motorPWMB, speed);
  digitalWrite(motorBDirPin, HIGH);
  Serial.println("left forward");
}

void moveRightForward(int speed) {
  analogWrite(motorPWMA, speed);
  digitalWrite(motorADirPin, HIGH);

  analogWrite(motorPWMB, speed / 2);
  digitalWrite(motorBDirPin, HIGH);
  Serial.println("right forward");
}

void moveLeftBackward(int speed) {
  analogWrite(motorPWMA, speed / 2);
  digitalWrite(motorADirPin, LOW);

  analogWrite(motorPWMB, speed);
  digitalWrite(motorBDirPin, LOW);
  Serial.println("left backward");
}

void moveRightBackward(int speed) {
  analogWrite(motorPWMA, speed);
  digitalWrite(motorADirPin, LOW);

  analogWrite(motorPWMB, speed / 2);
  digitalWrite(motorBDirPin, LOW);
  Serial.println("right backward");
}

void stopMotors() {
  analogWrite(motorPWMA, 0);
  analogWrite(motorPWMB, 0);
}

int calculateDistance() {
  long duration, distance;
  digitalWrite(trigGPO, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(trigGPO, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(trigGPO, LOW);
  duration = pulseIn(echoGPO, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print("********** Ultrasonic Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}

void setup() {
  RemoteXY_Init();
  Serial.begin(9600);
  
  pinMode(motorPWMA, OUTPUT);
  pinMode(motorPWMB, OUTPUT);
  pinMode(motorADirPin, OUTPUT);
  pinMode(motorBDirPin, OUTPUT);

  servo.attach(servoGPO);
  servo.write(90);
  delay(500);
  servo.write(0);
  delay(500);
  servo.write(180);
  delay(500);
  servo.write(90);

  pinMode(trigGPO, OUTPUT);
  pinMode(echoGPO, INPUT);

  // pinMode(led_power, OUTPUT);

  distance = calculateDistance();
  delay(100);
  distance = calculateDistance();
  delay(100);
  distance = calculateDistance();
  delay(100);
  distance = calculateDistance();
  delay(100);
}

void loop() {
  RemoteXY_Handler();
    
  // digitalWrite(led_power, (RemoteXY.power_led==0)?LOW:HIGH);

  if (RemoteXY.select_01==0) {
    servo.write(180);
  }
  else if (RemoteXY.select_01==1) {
    servo.write(90);
  }
  else if (RemoteXY.select_01==2) {
    servo.write(0);
  }

  if (RemoteXY.button_01 == 1){
    Forward();
  } else if (RemoteXY.button_02 == 1) {
    Right();
  } else if (RemoteXY.button_03 == 1) {
    Backward();
  } else if (RemoteXY.button_04 == 1) {
    Left();
  } else {
    Stop();
  }
  // int speedY = abs(RemoteXY.joystick_1_y);
  // int speedX = abs(RemoteXY.joystick_1_x);

  // if (RemoteXY.joystick_1_y > 0 && RemoteXY.joystick_1_x == 0) { // Moving forward
  //   moveForward(speedY * 2.55);
  // } else if (RemoteXY.joystick_1_y < 0 && RemoteXY.joystick_1_x == 0) { // Moving backward
  //   moveBackward(speedY * 2.55);
  // } else if (RemoteXY.joystick_1_x < 0 && RemoteXY.joystick_1_y > 0) { // Moving left forward
  //   moveLeftForward(speedY * 2.55);
  // } else if (RemoteXY.joystick_1_x > 0 && RemoteXY.joystick_1_y > 0) { // Moving right forward
  //   moveRightForward(speedY * 2.55);
  // } else if (RemoteXY.joystick_1_x < 0 && RemoteXY.joystick_1_y < 0) { // Moving left backward
  //   moveLeftBackward(speedY * 2.55);
  // } else if (RemoteXY.joystick_1_x > 0 && RemoteXY.joystick_1_y < 0) { // Moving right backward
  //   moveRightBackward(speedY * 2.55);
  // } else { // Stop
  //   stopMotors();
  // }

  // distance = calculateDistance();
}


void Left() {
  analogWrite(motorPWMA, motor_speed);
  analogWrite(motorPWMB, motor_speed);
  digitalWrite(motorADirPin, LOW);
  digitalWrite(motorBDirPin, HIGH);
}

void Right() {
  analogWrite(motorPWMA, motor_speed);
  analogWrite(motorPWMB, motor_speed);
  digitalWrite(motorADirPin, HIGH);
  digitalWrite(motorBDirPin, LOW);
}

void Forward() {
  analogWrite(motorPWMA, motor_speed);
  analogWrite(motorPWMB, motor_speed);
  digitalWrite(motorADirPin, HIGH);
  digitalWrite(motorBDirPin, HIGH);
}

void Backward() {
  analogWrite(motorPWMA, motor_speed);
  analogWrite(motorPWMB, motor_speed);
  digitalWrite(motorBDirPin, LOW);
  digitalWrite(motorADirPin, LOW);
}

void Stop() {
  analogWrite(motorPWMA, 0);
  analogWrite(motorPWMB, 0);
}



