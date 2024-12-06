#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD (I2C address is usually 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin assignments for IR sensors
#define IR_SENSOR1 A0
#define IR_SENSOR2 A1

// Traffic light pins for Road 1
#define R1_RED 2
#define R1_GREEN 3

// Traffic light pins for Road 2
#define R2_RED 4
#define R2_GREEN 5

// Traffic light pins for Road 3
#define R3_RED 6
#define R3_GREEN 7

void setup() {
  // Initialize IR sensor pins
  pinMode(IR_SENSOR1, INPUT);
  pinMode(IR_SENSOR2, INPUT);

  // Initialize traffic light pins
  pinMode(R1_RED, OUTPUT);
  pinMode(R1_GREEN, OUTPUT);
  pinMode(R2_RED, OUTPUT);
  pinMode(R2_GREEN, OUTPUT);
  pinMode(R3_RED, OUTPUT);
  pinMode(R3_GREEN, OUTPUT);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Start with all RED lights
  digitalWrite(R1_RED, HIGH);
  digitalWrite(R2_RED, HIGH);
  digitalWrite(R3_RED, HIGH);
}

void loop() {
  // Read traffic density from IR sensors
  int density1 = analogRead(IR_SENSOR1);
  int density2 = analogRead(IR_SENSOR2);

  // Road 1 Green, Others Red
  controlTraffic(R1_GREEN, R1_RED, "Road 1", density1, R2_RED, R3_RED);
  delay(density1 > 500 ? 10000 : 5000); // Longer delay if density is high

  // Road 2 Green, Others Red
  controlTraffic(R2_GREEN, R2_RED, "Road 2", density2, R1_RED, R3_RED);
  delay(density2 > 500 ? 10000 : 5000);

  // Road 3 Green, Others Red (Fixed time)
  controlTraffic(R3_GREEN, R3_RED, "Road 3", 0, R1_RED, R2_RED);
  delay(5000);
}

// Function to control traffic lights and display on LCD
void controlTraffic(int greenPin, int redPin, const char* roadName, int density, int otherRed1, int otherRed2) {
  // Turn green light ON for current road
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, LOW);

  // Turn red lights ON for other roads
  digitalWrite(otherRed1, HIGH);
  digitalWrite(otherRed2, HIGH);

  // Update LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(roadName);
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  if (density > 500) {
    lcd.print("10 sec");
  } else {
    lcd.print("5 sec");
  }

  // Wait for green light duration
  delay(density > 500 ? 10000 : 5000);

  // Turn green light OFF
  digitalWrite(greenPin, LOW);
  digitalWrite(redPin, HIGH);
}
