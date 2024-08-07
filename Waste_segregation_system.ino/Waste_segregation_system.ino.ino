#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Initialize the LCD display with I2C address 0x27 and dimensions 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define ir1 A1
#define sen1 A0 // Moisture sensor
#define buzzer 2
#define green 4
#define blue 5

// Define pins for ultrasonic sensors
const int trigPin1 = 9;
const int echoPin1 = 10;
const int trigPin2 = 11;
const int echoPin2 = 12;

int lvl1 = 0;
int lvl2 = 0;
long duration1, duration2, distance1, distance2, sensor1, sensor2;
int onetime = 0, onetime1 = 0;
int wet = 0, moisture = 0, object = 0, cabin2 = 0, c1 = 0, c2 = 0;

Servo myservo; // Create servo object to control a servo

void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight(); // Ensure backlight is on

  lcd.setCursor(0, 0);
  lcd.print("Automatic WASTE");
  lcd.setCursor(0, 1);
  lcd.print("Segregation sys");
  delay(3000);

  pinMode(ir1, INPUT);
  pinMode(sen1, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trigPin1, OUTPUT); // Set trigPin1 as an Output
  pinMode(echoPin1, INPUT); // Set echoPin1 as an Input
  pinMode(trigPin2, OUTPUT); // Set trigPin2 as an Output
  pinMode(echoPin2, INPUT); // Set echoPin2 as an Input
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  Serial.begin(9600); // Start serial communication

  digitalWrite(green, HIGH);
  digitalWrite(blue, HIGH);

  myservo.attach(3); // Attach the servo on pin 3 to the servo object
  myservo.write(75);
  delay(10);

  lcd.clear();
  lcd.print("Project By ");
  delay(300);
  lcd.clear();
  lcd.print("Group No 13");
  delay(1000);
  lcd.clear();
}

void loop() {
  // Clear the trigPin
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);

  // Set the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin1, HIGH);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);

  // Read the echoPin, returns the sound wave travel time in microseconds
  duration1 = pulseIn(echoPin1, HIGH);
  duration2 = pulseIn(echoPin2, HIGH);

  // Calculate the distance
  distance1 = duration1 * 0.034 / 2;
  distance2 = duration2 * 0.034 / 2;

  // Print the distance on the Serial Monitor
  Serial.print("Distance1: ");
  Serial.println(distance1);
  Serial.print("Distance2: ");
  Serial.println(distance2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WET      DRY");
  lcd.setCursor(2, 1);
  lcd.print(distance1);
  lcd.setCursor(11, 1);
  lcd.print(distance2);
  delay(1000);

  object = digitalRead(ir1);
  moisture = analogRead(sen1);

  if (object == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("-GARBAGE SENSE-");
    lcd.setCursor(0, 1);
    lcd.print("moisture: ");
    lcd.setCursor(12, 1);
    lcd.print(moisture);

    digitalWrite(buzzer, HIGH);
    delay(150);
    digitalWrite(buzzer, LOW);

    delay(1000);
    moisture = analogRead(sen1);
    Serial.print("moisture = ");
    Serial.println(moisture);
    delay(500);

    if (moisture > 700) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Dry GARBAGE");
      myservo.write(135);
      delay(1000);
      myservo.write(74);
      delay(2000);
      Serial.println("Dry");
    } else if (moisture < 750) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Wet GARBAGE");
      myservo.write(0);
      delay(1000);
      myservo.write(74);
      delay(2000);
      Serial.println("Wet");
      lcd.clear();
    }
  }
}

