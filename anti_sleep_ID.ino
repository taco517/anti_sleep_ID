#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

int motorPin1 = 4;
int motorPin2 = 7;

const unsigned long delayInterval = 5000;
const unsigned long blinkInterval = 500; 
unsigned long previousTime = 0; 
unsigned long previousBlinkTime = 0; 
const int ledPin = 13; 
bool ledState = false; 
bool delayInProgress = false;
bool motorState = false;

Adafruit_MPU6050 mpu;

void setup(void) {

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  Serial.begin(115200);

  while (!Serial)
    delay(10);

  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(100);

  pinMode(ledPin, OUTPUT);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.println(a.acceleration.z);
 
  float AccX = a.acceleration.x;
  float AccY = a.acceleration.y;
  float AccZ = a.acceleration.z;

  if (AccY > -9 && AccY < 2) {
    unsigned long currentTime = millis(); 

    if (!delayInProgress) {
      delayInProgress = true;
      previousTime = currentTime;
    }

    if (currentTime - previousTime >= delayInterval) {
      //  interval has passed, start the 5 sec motor delay
      if (currentTime - previousBlinkTime >= blinkInterval) {
        //  delay has passed, turn on motor with blink interval of 0.5 sec
        ledState = !ledState;
        motorState = !motorState;
        digitalWrite(ledPin, ledState); 
        digitalWrite(motorPin1, motorState);
        digitalWrite(motorPin2, motorState);
        previousBlinkTime = currentTime;
      }
    }
  } else {
    // reset the delay 
    delayInProgress = false;
    digitalWrite(ledPin, LOW);
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }

  delay(1000);                                          
  }