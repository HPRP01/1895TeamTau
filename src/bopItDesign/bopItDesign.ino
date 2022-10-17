// Bop-It Project Design
// University of Pittsburgh ECE1895
// Hudson Panning
// Ryan Kerber
// Gunnar Orth

#include <Wire.h>             //I2C
#include <Adafruit_GFX.h>     //OLED
#include <Adafruit_SSD1306.h> //OLED
#include <Adafruit_MPU6050.h> //GYRO
#include <Adafruit_Sensor.h>  //GYRO

// Define OLED Width and Height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define I/O pins
#define START_BUTTON 9
#define ACTION_BUTTON 8
#define PHOTORESISTOR A0
#define SPEAKER 10
#define LEDPIN 7

// Define I2C Addresses
#define DISPLAY_I2C 0x3C
#define GYRO_I2C 0x68

// Instantiate OLED Display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Instantiate Gyro
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  
  // OLED
    // Connect SCL to pin 26 (1st on right)
    // Connect SDA to pin 27 (2nd on right)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Start Button
    // Connect Start Button to D9 (pin 15 bottom right)
    pinMode(START_BUTTON, INPUT);

  // Action Button
    // Connect Action Button to D8 (pin 14 bottom left)
    pinMode(ACTION_BUTTON, INPUT);

  // Photoresistor
    // Connect photoresistor to pin A0 (pin 23 6 down right)
    pinMode(PHOTORESISTOR, INPUT);

  // Rotation Sensor
    // Connect SCL to pin 26 (1st on right)
    // Connect SDA to pin 27 (2nd on right)
    mpu.begin(MPU6050_SCLAE_2000DPS, MPU6050_RANGE_2G);

  // Speaker
    // Connect speaker to pin D10 (pin 16 2nd from bottom right)
    
  // LED for Start
    // Connect LED to pin D7 (pin 13 2nd from bottom left)
    pinMode(LEDPIN, OUTPUT);
}

void loop() {
  

}
