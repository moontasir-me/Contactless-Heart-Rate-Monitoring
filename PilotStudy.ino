#define TRIG_PIN 9  // Pin connected to the TRIG pin of the ultrasonic sensor
#define ECHO_PIN 10 // Pin connected to the ECHO pin of the ultrasonic sensor

long previousDistance = 0;  // To store the last measured distance
int pulseCount = 0;         // To count the number of pulses (changes > 3 cm)
unsigned long startTime;    // To track the start time for calculating pulses per minute
unsigned long pulsePerMinute = 0;  // Store the calculated pulses per minute

// Function to measure distance using the ultrasonic sensor
long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);  // Clear the trigger pin
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); // Set the trigger high for 10 microseconds
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH); // Measure the echo pulse duration
  long distance = duration * 0.034 / 2;    // Convert duration to distance in cm
  return distance;
}

void setup() {
  Serial.begin(9600);           // Initialize serial communication at 9600 baud
  pinMode(TRIG_PIN, OUTPUT);     // Set TRIG pin as output
  pinMode(ECHO_PIN, INPUT);      // Set ECHO pin as input

  startTime = millis();          // Record the start time when the program starts
  Serial.println("Distance measurement with pulse counting started");  // Initial message
}

void loop() {
  long distance = measureDistance();

  // Check if the change in distance is greater than 3 cm
  if (abs(distance - previousDistance) > 3) {
    pulseCount++;  // Count this as a pulse
    previousDistance = distance;  // Update the previous distance for the next comparison
  }

  // Calculate pulses per minute (update every 60 seconds)
  unsigned long elapsedTime = (millis() - startTime) / 1000;
  if (elapsedTime >= 60) {
    pulsePerMinute = pulseCount;  // Pulse count for the past minute
    pulseCount = 0;               // Reset pulse count
    startTime = millis();          // Reset the timer for the next minute

    // Send pulses per minute and distance to serial for Python to capture
    Serial.print("Pulses per minute: ");
    Serial.println(pulsePerMinute);
  }

  // Send the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.println(distance); // Distance output in cm
  
  // Optional: For Serial Plotter visualization, print only the distance value
  Serial.println(distance);  // This will be plotted in the Serial Plotter

  delay(500);  // Delay for half a second before taking the next measurement
}