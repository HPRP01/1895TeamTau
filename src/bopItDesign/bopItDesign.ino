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
    display.display();
    delay(1000);
    display.clearDisplay();

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
    mpu.begin(); 

  // Speaker
    // Connect speaker to pin D10 (pin 16 2nd from bottom right)
    
  // LED for Start
    // Connect LED to pin D7 (pin 13 2nd from bottom left)
    pinMode(LEDPIN, OUTPUT);
}

void loop() {
  int  turnTime    = 1000;
  bool playingGame = false;
  
  bool gameStarted = digitalRead(START_BUTTON);

  if(gameStarted == 1)
  {
    display.clearDisplay();
    bool playingGame = true;
    int  score       = 0;
  
    while(playingGame == true)
    {
      bool actionCompleted = false;
      int  action          = random(1,4);

      if(action == 1) {
        // Play Tone 1
        display.println("Push It");
        display.println(score);
        display.display();
      } else if(action == 2) {
        // Play Tone 2
        display.println("Cover It");
        display.println(score);
        display.display();
      } else {
        // Play Tone 3
        display.println("Spin It");
        display.println(score);
        display.display();
      }

      int startTime   = millis();
      int elapsedTime = millis() - startTime;
      while(elapsedTime < turnTime)
      {
        elapsedTime = millis() - startTime;
        
        if(action == 1)
        {
          // PUSH IT
          bool pushedIt = digitalRead(ACTION_BUTTON);

          if(pushedIt == 1)
          {
            actionCompleted = true;
            delay(200);
            break;
          }
        }
        else if(action == 2)
        {
          // COVER IT
        }
        else if(action == 3)
        {
          // SPIN IT
        }
      }
      
      if(actionCompleted == false)
      {
        // GAME OVER
        display.println("GAME OVER");
        display.println("Final Score: ");
        display.println(score);
        display.display();
      }

      score    = score + 1;
      turnTime = turnTime - 50;
    } //End While 
  } //End If gameStarted
}
