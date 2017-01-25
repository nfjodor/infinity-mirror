// ULTRASONIC SENSOR CONFIG
// http://randomnerdtutorials.com
 
#define ULTRASONIC_TRIGGER_PIN 12 // green jumper
#define ULTRASONIC_ECHO_PIN 11 // blue jumper
#define MIN_MEASUREMENT_DISTANCE 10 // in cm
#define MAX_MEASUREMENT_DISTANCE 80 // in cm

#define SIGNAL_PIN 13

// GLOBAL PARAMETERS FOR COMPUTING
long echo_duration, cm;
bool inRange = false;
int avg[20];
int avgPoint = 0;
int avgCount = 20;
int weight = 0;

void setup() {
  Serial.begin(9600);
  
  // initialize average array
  for (int i = 0; i < avgCount; i++) {
    avg[i] = 0;
  }
  
  // define inputs and outputs
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
}

void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(ULTRASONIC_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  echo_duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH, 110000);
 
  // convert the time into a distance in centimeters
  cm = (echo_duration/2) / 29.1;
  
  // average distance data
  inRange = (cm > MIN_MEASUREMENT_DISTANCE && cm < MAX_MEASUREMENT_DISTANCE);
  avg[avgPoint++] = inRange ? 1 : 0;
  if (avgPoint == avgCount) {
    avgPoint = 0;
  }
  
  // weight of the last few measurements (values of 'inRange')
  weight = 0;
  for (int i = 0; i < avgCount; i++) {
    weight += avg[i];
  }
  
  //Serial.println(weight);
  
  // trigger controller
  if (weight > avgCount / 2) {
    digitalWrite(SIGNAL_PIN, HIGH);
  } else {
    digitalWrite(SIGNAL_PIN, LOW);
  }
    
  delay(60-weight*2); // increase this to increse latency
}

