# ESP8266 Remote Control Car with Servo and Ultrasonic Sensor

This project demonstrates how to use an ESP8266 microcontroller to control a car with motors, a servo for steering, and an ultrasonic sensor for distance measurement. The control interface is provided by RemoteXY, which allows for easy control over Wi-Fi.

## RemoteXY

[RemoteXY](https://remotexy.com/) is a platform that allows you to create graphical user interfaces for microcontroller projects without having to write complex code. You can design your interface using a visual editor and generate code that integrates with your project. RemoteXY supports various connection modes, including Bluetooth, Wi-Fi, and USB.

In this project, RemoteXY is used to create a Wi-Fi access point and provide a simple interface to control the car's movements and servo positions. The generated interface includes buttons and selectors that map to the ESP8266's GPIO pins.

## Hardware Requirements

- ESP8266 (e.g., NodeMCU)
- Motor Driver
- Servo Motor
- Ultrasonic Sensor
- Motors and Wheels
- Power Supply

## Software Requirements

- Arduino IDE with ESP8266 support
- Libraries:
  - `ESP8266WiFi`
  - `Wire`
  - `Servo`
  - `RemoteXY`

## Circuit Diagram

Connect the components as follows:

- **Motor Driver**

  - Motor A PWM to D1
  - Motor A Direction to D3
  - Motor B PWM to D2
  - Motor B Direction to D4

- **Servo Motor**

  - Control to D6

- **Ultrasonic Sensor**
  - Trig to D7
  - Echo to D8

## Usage

1. Install the required libraries in the Arduino IDE.
2. Connect the ESP8266 to your computer and upload the code.
3. Open the Serial Monitor to view debug messages.
4. The device will create a Wi-Fi access point named "BOT" with the password "123456789".
5. Connect to the access point using the RemoteXY app and control the car.

## Code Explanation

- `setup()`: Initializes the Wi-Fi connection, RemoteXY interface, and sensors.
- `loop()`: Continuously handles RemoteXY events and controls the motors and servo based on the input.
- `moveForward(int speed)`: Moves the car forward at the specified speed.
- `moveBackward(int speed)`: Moves the car backward at the specified speed.
- `moveLeftForward(int speed)`: Moves the car left forward at the specified speed.
- `moveRightForward(int speed)`: Moves the car right forward at the specified speed.
- `moveLeftBackward(int speed)`: Moves the car left backward at the specified speed.
- `moveRightBackward(int speed)`: Moves the car right backward at the specified speed.
- `stopMotors()`: Stops the motors.
- `calculateDistance()`: Calculates the distance using the ultrasonic sensor.

## License

This project is licensed under the MIT License.
