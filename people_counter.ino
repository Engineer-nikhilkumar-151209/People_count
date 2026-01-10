//code by Engineer-nikhilkumar-151209
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3); // TX, RX pins for Bluetooth

const int sensorA = 4;
const int sensorB = 5;
const int ledPin = 13;

int count = 0;
int inCount = 0;
int outCount = 0;

bool flagA = false;
bool flagB = false;

void setup() {
  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  BTSerial.begin(9600);
  printDisplay(); // Run once at start
}

void loop() {
  // Logic for IN (A then B)
  if (digitalRead(sensorA) == LOW && !flagA && !flagB) {
    flagA = true;
    delay(100); 
  }
  if (digitalRead(sensorB) == LOW && flagA) {
    inCount++; count++;
    flagA = false;
    blinkLED();
    printDisplay();
    delay(500); 
  }

  // Logic for OUT (B then A)
  if (digitalRead(sensorB) == LOW && !flagB && !flagA) {
    flagB = true;
    delay(100);
  }
  if (digitalRead(sensorA) == LOW && flagB) {
    outCount++; count = max(0, count - 1);
    flagB = false;
    blinkLED();
    printDisplay();
    delay(500);
  }
}

void blinkLED() {
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
}

// THIS IS THE ONLY VERSION OF printDisplay() YOU SHOULD HAVE
void printDisplay() {
  // 1. Clean view for Serial Monitor
  Serial.print("IN: "); Serial.print(inCount);
  Serial.print(" | OUT: "); Serial.print(outCount);
  Serial.print(" | TOTAL: "); Serial.println(count);

  // 2. Data for Web Dashboard (Comma Separated)
  BTSerial.print(inCount);
  BTSerial.print(",");
  BTSerial.print(outCount);
  BTSerial.print(",");
  BTSerial.println(count); 
}

