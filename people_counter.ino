//code by Engineer-nikhilkumar-151209
// People Counter using IR Sensors and Bluetooth
// IR Sensor 1: Entry sensor (outside)
// IR Sensor 2: Exit sensor (inside)

const int entrySensor = 2;  // IR sensor at entry
const int exitSensor = 3;   // IR sensor at exit

int peopleCount = 0;
bool sensor1Triggered = false;
bool sensor2Triggered = false;
bool personEntering = false;
bool personExiting = false;

unsigned long lastTriggerTime = 0;
const unsigned long debounceDelay = 1000; // 1 second debounce

void setup() {
  Serial.begin(9600); // For Bluetooth communication
  
  pinMode(entrySensor, INPUT);
  pinMode(exitSensor, INPUT);
  
  Serial.println("START");
  Serial.print("COUNT:");
  Serial.println(peopleCount);
}

void loop() {
  int entry = digitalRead(entrySensor);
  int exit = digitalRead(exitSensor);
  
  unsigned long currentTime = millis();
  
  // IR sensors output LOW when object detected
  // Entry detection sequence
  if (entry == LOW && !sensor1Triggered && !personExiting) {
    sensor1Triggered = true;
    personEntering = true;
    lastTriggerTime = currentTime;
  }
  
  if (sensor1Triggered && exit == LOW && personEntering) {
    sensor2Triggered = true;
    peopleCount++;
    sendCount();
    resetSensors();
  }
  
  // Exit detection sequence
  if (exit == LOW && !sensor2Triggered && !personEntering) {
    sensor2Triggered = true;
    personExiting = true;
    lastTriggerTime = currentTime;
  }
  
  if (sensor2Triggered && entry == LOW && personExiting) {
    sensor1Triggered = true;
    if (peopleCount > 0) {
      peopleCount--;
    }
    sendCount();
    resetSensors();
  }
  
  // Reset if sequence not completed within timeout
  if ((sensor1Triggered || sensor2Triggered) && 
      (currentTime - lastTriggerTime > 2000)) {
    resetSensors();
  }
  
  // Listen for commands from Bluetooth
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command == "GET") {
      sendCount();
    } else if (command == "RESET") {
      peopleCount = 0;
      sendCount();
    }
  }
  
  delay(50);
}

void sendCount() {
  Serial.print("COUNT:");
  Serial.println(peopleCount);
}

void resetSensors() {
  sensor1Triggered = false;
  sensor2Triggered = false;
  personEntering = false;
  personExiting = false;
}