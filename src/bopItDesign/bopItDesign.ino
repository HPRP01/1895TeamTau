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

// Define I/O pins
#define START_BUTTON 9
#define ACTION_BUTTON 8
#define PHOTORESISTOR A0
#define SPEAKER 10

#define LEDPIN 7
#define LEDPIN_FAIL 6

#define COVER_IT_CUTOFF 200

// Define I2C Addresses
#define DISPLAY_I2C 0x3C

// Instantiate OLED Display
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// Instantiate Gyro
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600);

  if (!mpu.begin()) {
    digitalWrite(LEDPIN, HIGH);
    while (1) {
      delay(10);
    }
  }
  
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

  // Speaker
    // Connect speaker to pin D10 (pin 16 2nd from bottom right)
    
  // LED for Start
    // Connect LED to pin D7 (pin 13 2nd from bottom left)
    pinMode(LEDPIN, OUTPUT);
    pinMode(LEDPIN_FAIL, OUTPUT);
}

void loop() {
  display.clearDisplay();
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  
  int  turnTime    = 2000;
  bool playingGame = false;
  
  bool gameStarted = digitalRead(START_BUTTON);

  if(gameStarted == 1)
  {
    bool playingGame = true;
    int  score       = 0;
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0,0);
    digitalWrite(LEDPIN_FAIL, LOW);
    digitalWrite(LEDPIN, LOW);
    delay(1000);
  
    while(playingGame == true)
    {
      bool actionCompleted = false;
      int  action          = random(1,4);
      sensors_event_t a, g, temp;
      display.clearDisplay();
      display.setCursor(0,0);
      

      if(action == 1) {
        display.println(F("Push It"));
        display.println(score);
        display.display();
        
        tone(10, 523, 500);
        delay(500);
        tone(10, 1047, 500);
        delay(500);
        noTone(10);
      } else if(action == 2) {
        // Play Tone 2
        display.println(F("Cover It"));
        display.println(score);
        display.display();
        
        tone(10, 523, 500);
        delay(500);
        tone(10, 523, 500);
        delay(500);
        noTone(10);
      } else {
        display.println(F("Spin It"));
        display.println(score);
        display.display();
        
        tone(10, 523, 500);
        delay(500);
        tone(10, 262, 500);
        delay(500);
        noTone(10);
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

          if(analogRead(PHOTORESISTOR) < COVER_IT_CUTOFF)
          {
            actionCompleted = false;
            break;
          }

          mpu.getEvent(&a, &g, &temp);
          if(a.acceleration.x > 2)
          {
            actionCompleted = false;
            break;
            
          }

          if(pushedIt == 1)
          {
            actionCompleted = true;
            break;
          }
        }
        else if(action == 2)
        {
          // COVER IT
          bool pushedIt = digitalRead(ACTION_BUTTON);
          if(pushedIt == 1)
          {
            actionCompleted = false;
            break;
          }

          mpu.getEvent(&a, &g, &temp);
          if(a.acceleration.x > 2)
          {
            actionCompleted = false;
            break;
            
          }
          
          if(analogRead(PHOTORESISTOR) < COVER_IT_CUTOFF)
          {
            actionCompleted = true;
            break;
          }
        }
        else if(action == 3)
        {
          if(analogRead(PHOTORESISTOR) < COVER_IT_CUTOFF)
          {
            actionCompleted = false;
            break;
          }

          bool pushedIt = digitalRead(ACTION_BUTTON);
          if(pushedIt == 1)
          {
            actionCompleted = false;
            break;
          }
          
          mpu.getEvent(&a, &g, &temp);
          if(a.acceleration.x > 2)
          {
            actionCompleted = true;
            break;
            
          }
          // SPIN IT
          
        }
      }
      
      if(actionCompleted == false)
      {
        // GAME OVER
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(F("GAME OVER"));
        display.println("Score: ");
        display.println(score);
        display.display();
        digitalWrite(LEDPIN_FAIL, HIGH);
        break;
      }

      score    = score + 1;
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("");
      display.println(score);
      display.display();
      digitalWrite(LEDPIN, HIGH);
      delay(turnTime - elapsedTime);
      digitalWrite(LEDPIN, LOW);

      if(score == 5)
      {
        digitalWrite(LEDPIN, HIGH);
        digitalWrite(LEDPIN_FAIL, HIGH);
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("You Win!");
        display.println(score);
        display.display();
        break;
      }
      
      turnTime = turnTime - 10;
    } //End While 
  } //End If gameStarted
}
