
#include <Arduino_LSM9DS1.h>

// Acceleration threshold for triggering a recording
const float motion_threshold = 0.3;

// Num samples to record per gesture. Accelerometer and Gyroscope updates at
// 104hz, so 120 samples will record a second
const int num_samples_per_gesture = 104;

// Time the device needs to be held still before we start a new recording
const unsigned int wait_for_idle_timeout_ms = 500;

// State variables
int samples_read = 0;
bool is_recording = false;
bool wait_for_idle = false;
unsigned long wait_for_idle_timer = 0;

void setup() {

  // Setup pins for RGB LED
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // Initial serial at 9600 baud
  Serial.begin(9600);

  // Wait for serial connection
  while (!Serial)
    ;

  if (!IMU.begin()) {
    Serial.println("Error: Failed to initialize IMU");
    while (true)
      ;
  }

  // Print our header - accelorameter.x, .y, .z and gyroscope.x, .y and .z
  Serial.println("ax,ay,az,gx,gy,gz");
}

inline void normalize(float &v1, float &v2, float &v3, float min, float max) {
  const float range = max - min;
  v1 /= range;
  v2 /= range;
  v3 /= range;
}

void rgbLedRed() {
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}

void rgbLedGreen() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, HIGH);
}

void rgbLedBlue() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, LOW);
}

// Queue recording
void arm_recording() {
  // Allow recording
  wait_for_idle = false;

  // Signal state with LED
  rgbLedGreen();
}

void begin_recording() {
  // Enable recording
  is_recording = true;

  // Reset sample count
  samples_read = 0;

  // Signal state with LED
  rgbLedRed();
}

void end_recording() {
  // Disable recording
  is_recording = false;

  // Signal to wait until low motion has been detected
  wait_for_idle = true;
  wait_for_idle_timer = millis();

  // Signal state with LED
  rgbLedBlue();
}

// Main loop
void loop() {

  float ax, ay, az, gx, gy, gz;

  while (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {

    // Read acceleration
    IMU.readAcceleration(ax, ay, az);

    // Acceleration ranges between -4g and 4g, normalize to between -1. and 1.
    normalize(ax, ay, az, -4., 4.);

    if (!is_recording) {
      float sumAcceleration = abs(ax) + abs(ay) + abs(az);

      // If our acceleration is above a threshold, and we're not already
      // recording then start recording
      if (sumAcceleration > motion_threshold) {
        if (wait_for_idle) {
          // Too much motion, reset timer
          wait_for_idle_timer = millis();
        } else {
          begin_recording();
        }
      } else {
        if (millis() - wait_for_idle_timer > wait_for_idle_timeout_ms) {
          arm_recording();
        }
      }
    }

    if (is_recording) {

      // Read angular velocity
      IMU.readGyroscope(gx, gy, gz);

      // Acceleration ranges between -2000dps and 2000dps, normalize to between
      // -1. and 1.
      normalize(gx, gy, gz, -2000., 2000.);

      // Print out the data with 12 decimal places
      Serial.print(ax, 12);
      Serial.print(',');
      Serial.print(ay, 12);
      Serial.print(',');
      Serial.print(az, 12);
      Serial.print(',');
      Serial.print(gx, 12);
      Serial.print(',');
      Serial.print(gy, 12);
      Serial.print(',');
      Serial.print(gz, 12);
      Serial.println();

      samples_read++;

      if (samples_read == num_samples_per_gesture) {
        end_recording();

        // Print an empty line to separate the recordings
        Serial.println();
      }
    }
  }
}
