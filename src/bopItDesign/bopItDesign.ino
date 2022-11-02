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

#define LEDPIN 7      // SUCCESS LED
#define LEDPIN_FAIL 6 // FAILURE LED

#define COVER_IT_CUTOFF 200 // Configure cutoff for Photoresistor

// Define I2C Addresses
#define DISPLAY_I2C 0x3C

// Instantiate OLED Display
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// Instantiate Gyro
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600);

  // Initialize MPU (accelerometer/gyroscope)
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

    // Set output LEDs LOW
    digitalWrite(LEDPIN_FAIL, LOW);
    digitalWrite(LEDPIN, LOW);

    // Wait for game to start
    delay(1000);
  
    while(playingGame == true)
    {
      sensors_event_t a, g, temp;

      // Initialize action completed to false
      bool actionCompleted = false;

      // Pick random action
      int  action          = random(1,4);

      // Clear display
      display.clearDisplay();
      display.setCursor(0,0);
      

      if(action == 1) {
        // Display PUSH IT command
        display.println(F("Push It"));
        display.println(score);
        display.display();

        // Give tone MID -> HIGH
        tone(10, 523, 500);
        delay(500);
        tone(10, 1047, 500);
        delay(500);
        noTone(10);
      } else if(action == 2) {
        // Display COVER IT command
        display.println(F("Cover It"));
        display.println(score);
        display.display();

        // Give tone MID -> MID
        tone(10, 523, 500);
        delay(500);
        tone(10, 523, 500);
        delay(500);
        noTone(10);
      } else {
        // Display SPIN IT command
        display.println(F("Spin It"));
        display.println(score);
        display.display();

        // Give tone MID -> LOW
        tone(10, 523, 500);
        delay(500);
        tone(10, 262, 500);
        delay(500);
        noTone(10);
      }

      // Set start time to current time
      int startTime   = millis();
      int elapsedTime = millis() - startTime;

      // While time still left in turn
      while(elapsedTime < turnTime)
      {
        // Set elapsed time to current time - start time
        elapsedTime = millis() - startTime;

        // PUSH IT
        if(action == 1)
        {
          bool pushedIt = digitalRead(ACTION_BUTTON);

          // If cover it -> go to game over
          if(analogRead(PHOTORESISTOR) < COVER_IT_CUTOFF)
          {
            actionCompleted = false;
            break;
          }

          // If spin it -> go to game over
          mpu.getEvent(&a, &g, &temp);
          if(a.acceleration.x > 2)
          {
            actionCompleted = false;
            break;
            
          }

          // If push it -> go to success
          if(pushedIt == 1)
          {
            actionCompleted = true;
            break;
          }
        }

        // COVER IT
        else if(action == 2)
        {
          // If push it -> go to game over
          bool pushedIt = digitalRead(ACTION_BUTTON);
          if(pushedIt == 1)
          {
            actionCompleted = false;
            break;
          }

          // If spin it -> go to game over
          mpu.getEvent(&a, &g, &temp);
          if(a.acceleration.x > 2)
          {
            actionCompleted = false;
            break;
            
          }

          // If cover it -> go to success
          if(analogRead(PHOTORESISTOR) < COVER_IT_CUTOFF)
          {
            actionCompleted = true;
            break;
          }
        }

        // SPIN IT
        else if(action == 3)
        {
          // If Cover it -> go to game over
          if(analogRead(PHOTORESISTOR) < COVER_IT_CUTOFF)
          {
            actionCompleted = false;
            break;
          }

          // If push it -> go to game over
          bool pushedIt = digitalRead(ACTION_BUTTON);
          if(pushedIt == 1)
          {
            actionCompleted = false;
            break;
          }

          // If spin it -> go to success
          mpu.getEvent(&a, &g, &temp);
          if(a.acceleration.x > 2)
          {
            actionCompleted = true;
            break;
            
          }
        }
      }

      // GAME OVER
      // If action was not completed or incorrect action
      if(actionCompleted == false)
      {
        // Display final score and Game Over message
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(F("GAME OVER"));
        display.println("Score: ");
        display.println(score);
        display.display();

        // Set failure LED HIGH
        digitalWrite(LEDPIN_FAIL, HIGH);
        break;
      }

      // Increase Score
      score = score + 1;

      // Clear instruction from display and show new score
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("");
      display.println(score);
      display.display();
      
      // Set success LED HIGH
      digitalWrite(LEDPIN, HIGH);

      // Wait for the rest of turn time then set success LED LOW
      delay(turnTime - elapsedTime);
      digitalWrite(LEDPIN, LOW);

      // Decrease the turn time be 10ms at end of round
      turnTime = turnTime - 10;

      // Check for if score limit has been reached (set to 99)
      if(score == 99)
      {
        // Set both output LEDs HIGH
        digitalWrite(LEDPIN, HIGH);
        digitalWrite(LEDPIN_FAIL, HIGH);

        // Write score and you win to display
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("You Win!");
        display.println(score);
        display.display();

        // Break out of game loop
        break;
      }
    } //End While 
  } //End If gameStarted
}
