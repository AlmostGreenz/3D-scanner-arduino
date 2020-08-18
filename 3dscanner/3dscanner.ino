/* To run this program, upload it to the Arduino, then open the serial port.
   Next, place the object in the centre of the scanning area and place your
   hand ontop of the sensor as you will be required to provide some force
   after every 360 degree rotation in order to help the motor. Then, type
   scan into the serial monitor and wait for the scans to finish. Once the
   scan is completed, your measurements will be printed in the form x y z.
*/

// Initialize Variables
String message; // Output beginning data
int yCount; // Number of y decreases
bool scanning; // If scan is in progress

int precision = 44; // # of rotations for 360 degrees
int yLevels = 5; // Number of vertical movements

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Initialize the serial port
  
  // Horizontal Motor Pins
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);

  // Vertical Motor Pins
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(3, OUTPUT);

  message = "";
  yCount = 0;
  scanning = false;
}

void scan() { // Configures variables to start measuring
  reset();
  scanning = true;
  message += yLevels;
  message += "\n";
  message += precision;
  message += "\n";
}

void reset() { // Resets relevant variables
  yCount = 0;
  message = "";
  scanning = false;
}

void measure() {
  for (int i = 0; i < precision; i++) {
    // Turns on horizontal motor
    digitalWrite(2, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(7, HIGH);
    delay(100);
    // Turns off horizontal motor
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);

    // Courtesy of http://www.instructables.com/id/How-to-Use-the-Sharp-IR-Sensor-GP2Y0A41SK0F-Arduin/
    float volts = analogRead(A0)*0.0048828125;  // value from sensor * (5/1024)
    float distance = 13*pow(volts, -1); // worked out from datasheet graph
    
    if (distance <= 30.0) { // Checks if within valid range (of <= 30.0 cm)
      Serial.print(cos((i / float(precision)) * 6.28318530718) * (14 - distance));   // calculate & print x coordinate
      Serial.print(" ");
      Serial.print((yLevels - yCount - 1) * 1.4);                                    // calculate & print y coordinate
      Serial.print(" ");
      Serial.print(sin((i / float(precision)) * 6.28318530718) * (14 - distance));   // calculate & print z coordinate
      Serial.print("\n");
    } else {
      Serial.print("-1 -1 -1\n"); // If invalid measurement
    }
    delay(100);
  }

  // Moves sensor apparatus down
  // Turns on vertical motor
  digitalWrite(8, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  analogWrite(3, 255);
  analogWrite(11, 255);
  delay(250); // Delay to move
  // Turns off vertical motor
  digitalWrite(8, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  analogWrite(11, 0);
  analogWrite(3, 0);
  yCount++; // Increases the count of verticle movements

  delay(3000);
}

void loop() {
  String token = Serial.readStringUntil('\n'); // Checks for input
  
  if (token == "scan") { // Waits for signal to start (an input of "scan")
    scan();             // If true, configures variables to start measuring
    Serial.print(message); // Prints specifications

  }

  if (scanning) {
    measure(); // Runs one cycle of the scanner
  }

  if (yCount == yLevels) { // Checks if the sensor has reached y = 0
                           // If so, ends measurements
    reset();
  }
}


