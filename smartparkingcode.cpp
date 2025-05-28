#include <MFRC522.h> //Main library for interface with RFID reader
#include <MFRC522Extended.h> //Extends previous library functionality
#include <deprecated.h> //Allows compiler to generate warnings if code is old/should no longer be used
#include <require_cpp11.h> //Ensures compiler works with C++11 or later


#include <Servo.h> //Servo motor library
#include <Wire.h> //I2C communication library
#include <LiquidCrystal_I2C.h> //Ext of LCD library, supports I2C protocol
#include <SPI.h> //Serial Peripheral Interface library, high-speed data transfer
#include <MFRC522.h> //Main library for interface with RFID reader


// Define pins for IR sensors, exit sensor, and servo motor
#define IR_SENSOR_PIN 4  // IR sensor pin (decrements slot count)
#define EXIT_SENSOR_PIN 2  // Exit sensor pin (increments slot count)
#define SERVO_PIN 3  // Servo motor pin


// Define pins for individual car presence sensors
#define ir_enter 2 //pin 2
#define ir_back 4 //pin 4
#define ir_car1 5 //pin 5
#define ir_car2 6 //pin 6
#define ir_car3 7 //pin 7
#define ir_car4 8 //pin 8


// Initialize servo motor and LCD display
Servo servoMotor; //declaring servo motor
LiquidCrystal_I2C lcd(0x27, 20, 4); //establish LCD parameters


// Define pins for MFRC522 RFID reader
#define SS_PIN 10  // Slave Select Pin
#define RST_PIN 9  // Reset Pin


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance


// Initialize variables for parking slots, slot count, and sensor values
int S1 = 0, S2 = 0, S3 = 0, S4 = 0;
int slot = 4; // Initial number of available slots
int sensorValue;
bool carEntering = false;
bool carExiting = false;
unsigned long gateOpenTime = 0;
const unsigned long gateOpenDuration = 3000; // 3 seconds


// Define authorized UIDs for RFID cards
byte authorizedTags[2][4] = {
  {0x53, 0xC5, 0x16, 0xC8},  // Authorized UID 1
  {0xC6, 0x3B, 0x0C, 0x7A}   // Authorized UID 2
};


bool accessGranted = false;


void setup() {
  Serial.begin(9600); // Initialize serial communication


  // Set pin modes for IR sensors, exit sensor, and car presence sensors
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(EXIT_SENSOR_PIN, INPUT);
  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);
  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_car4, INPUT);


  // Attach servo motor and set initial position
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(90); // Set initial position


  // Initialize SPI bus and MFRC522 RFID reader
  SPI.begin();
  mfrc522.PCD_Init();


  // Initialize and clear LCD display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 2);
  lcd.print("      System      ");
  delay(2000);
  lcd.clear();
}


void loop() {
  // Read sensor values
  sensorValue = digitalRead(IR_SENSOR_PIN);
  int exitSensorValue = digitalRead(EXIT_SENSOR_PIN);


  // Process servo movement only if access is granted
  if (accessGranted) {
    if (exitSensorValue == HIGH && sensorValue == LOW) { // Check if exit sensor is triggered first
      // Decrement slot count when a car exits
      if (!carEntering && !carExiting) {
        if (slot < 4) {
          slot++;
        }
        servoMotor.write(0); // Open gate if a car is exiting
        gateOpenTime = millis(); // Record the time when the gate opened
        carExiting = true;
      }
    } else if (sensorValue == HIGH && exitSensorValue == LOW) { // Check if entry sensor is triggered
      // Increment slot count when a car enters
      if (!carEntering && !carExiting) {
        if (slot > 0) {
          slot--;
        } else {
          // Display "Sorry Parking Full" if no slots are available
          lcd.setCursor(0, 0);
          lcd.print(" Sorry Parking Full ");
          delay(1500);
          lcd.clear();
          return; // Exit the loop without moving the servo
        }
        servoMotor.write(0); // Rotate servo if a car is entering
        gateOpenTime = millis(); // Record the time when the gate opened
        carEntering = true;
      }
    } else {
      // Close the gate after the delay
      if ((carEntering || carExiting) && millis() - gateOpenTime >= gateOpenDuration) {
        servoMotor.write(90);
        carEntering = false;
        carExiting = false;
      }
    }
  }


  // Update LCD display with sensor readings
  Read_Sensor();
  Update_LCD();


  // Look for new RFID cards and compare UID with authorized tags
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    byte currentUid[4] = {0};


    for (byte i = 0; i < 4; i++) {
      currentUid[i] = mfrc522.uid.uidByte[i];
    }


    accessGranted = false;
    for (int i = 0; i < 2; i++) {
      if (compareUid(currentUid, authorizedTags[i])) {
        accessGranted = true;
        Serial.println("Access granted!");
        break;
      }
    }


    if (!accessGranted) {
      Serial.println("Access denied!");
    }


    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1(); // Stop encryption on PCD
  }
}


// Function to read the status of each parking slot
void Read_Sensor() {
  S1 = digitalRead(ir_car1) == 0 ? 1 : 0;
  S2 = digitalRead(ir_car2) == 0 ? 1 : 0;
  S3 = digitalRead(ir_car3) == 0 ? 1 : 0;
  S4 = digitalRead(ir_car4) == 0 ? 1 : 0;
}


// Function to update LCD display with sensor readings
void Update_LCD() {
  lcd.setCursor(0, 0);
  lcd.print("   # of Slots:   ");
  lcd.print(constrain(slot, 0, 4));


  lcd.setCursor(0, 1);
  lcd.print("S1: ");
  lcd.print(S1 == 1 ? "Full " : "Empty");
  lcd.setCursor(10, 1);
  lcd.print("S2: ");
  lcd.print(S2 == 1 ? "Full " : "Empty");


  lcd.setCursor(0, 2);
  lcd.print("S3: ");
  lcd.print(S3 == 1 ? "Full " : "Empty");
  lcd.setCursor(10, 2);
  lcd.print("S4: ");
  lcd.print(S4 == 1 ? "Full " : "Empty");
}


// Function to compare UIDs
bool compareUid(byte uid1[], byte uid2[]) {
  for (int i = 0; i < 4; i++) {
    if (uid1[i] != uid2[i]) {
      return false;
    }
  }
  return true;
}




