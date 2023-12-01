#include <Arduino.h>
#include <ESP32Servo.h>


#include <WiFi.h>
#include <WiFiMulti.h>

#include <SocketIoClient.h>

#define USE_SERIAL Serial

WiFiMulti WiFiMulti;
SocketIoClient webSocket;
const byte led_gpio = 32;


// CONST VARIABLES
const char *ssid = "ethezus iPhone";
const char *pass = "ethansmith";
const char *HOST = "ws://ec2-18-118-47-118.us-east-2.compute.amazonaws.com";

Servo myServo;
int servoPosition = 0;    // variable to store the servo position
int servoPin = 13;


void event(const char *payload, size_t length){
    USE_SERIAL.printf("[Socket Message]: %s\n", payload);
    digitalWrite(led_gpio, HIGH);   // Turn the LED on (HIGH is the voltage level)
    delay(2000);                   // Wait for a second
    digitalWrite(led_gpio, LOW);

    // Turn the servo 180 degrees
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);    // tell servo to go to position in variable 'pos'
	}

    // servoPosition += 90;
    // if (servoPosition > 180) {
    //     servoPosition = 180; // Limit the servo to 180 degrees
    // }
    // myServo.write(servoPosition);

    digitalWrite(led_gpio, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(2000);                       // wait for a second
    digitalWrite(led_gpio, LOW);
    
    //Wait ten seconds before servo goes back to locked
    delay(10000); 
    //Reset back to 0
	for (pos = 180; pos >= 0; pos -= 1) { 
		myservo.write(pos);    // tell servo to go to position in variable 'pos'
	}
}

void setup(){
    USE_SERIAL.begin(9600);

    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for (uint8_t t = 4; t > 0; t--){
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }   
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 500, 2400); // attaches the servo on pin 13 to the servo object
    myServo.write(servoPosition);  // Set the initial servo position
    // Connect to WIFI
    WiFiMulti.addAP(ssid, pass);

    while (WiFiMulti.run() != WL_CONNECTED){
        delay(100);
    }

    // Receive events from server
    webSocket.on("unlock_door", event);

    webSocket.begin("ec2-18-118-47-118.us-east-2.compute.amazonaws.com", 80);
    pinMode(led_gpio, OUTPUT);

}

int count = 0;

void loop(){
    webSocket.loop();
}