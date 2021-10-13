
#include <Arduino_LSM9DS1.h>

#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>
#include "model.h"

// array to map gesture index to a name
const char *GESTURES[] = { /* this needs to be filled in */ };

const int NUM_GESTURES = sizeof(GESTURES) / sizeof(GESTURES[0]);

// Tensorflow global variables

// global variables used for TensorFlow Lite (Micro)
tflite::MicroErrorReporter tflErrorReporter;

// pull in all the TFLM ops, you can remove this line and
// only pull in the TFLM ops you need, if would like to reduce
// the compiled size of the sketch.
tflite::AllOpsResolver tflOpsResolver;

const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

// Create a static memory buffer for TFLM, the size may need to
// be adjusted based on the model you are using
constexpr int tensorArenaSize = 8 * 1024;
byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));


// Acceleration threshold for triggering a recording
const float motion_threshold = 0.3; // ! Keep the same as when you captured it

// Num samples to record per gesture. Accelerometer and Gyroscope updates at 104hz, so 120 samples
// will record a second
const int num_samples_per_gesture = 104; // ! Keep the same as when you captured it

// Time the device needs to be held still before we start a new recording
const unsigned int wait_for_idle_timeout_ms = 500;

// State variables
int samples_read = 0;
bool is_recording = false;
bool wait_for_idle = false;
unsigned long wait_for_idle_timer = 0;

void setup(){
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
  Serial.println("Ready to run inference!");

  // get the TFL representation of the model byte array
  tflModel = tflite::GetModel(model);
  if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema mismatch!");
    while (1)
      ;
  }

  // Create an interpreter to run the model
  tflInterpreter =
      new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena,
                                   tensorArenaSize, &tflErrorReporter);

  // Allocate memory for the model's input and output tensors
  tflInterpreter->AllocateTensors();

  // Get pointers for the model's input and output tensors
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);
}

inline void normalize(float& v1, float& v2, float& v3, float min, float max){
  const float range = max - min;
  v1 /= range;
  v2 /= range;
  v3 /= range;
}


void rgbLedRed()
{
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}

void rgbLedGreen()
{
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, HIGH);
}

void rgbLedBlue()
{
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, LOW);
}

// Queue recording
void arm_recording(){
  // Allow recording
  wait_for_idle = false;

  // Signal state with LED
  rgbLedGreen();
}

void begin_recording(){
  // Enable recording
  is_recording = true;

  // Reset sample count
  samples_read = 0;

  // Signal state with LED
  rgbLedRed();
}

void end_recording(){
  // Disable recording
  is_recording = false;

  // Signal to wait until low motion has been detected
  wait_for_idle = true;
  wait_for_idle_timer = millis();

  // Signal state with LED
  rgbLedBlue();
}

// Main loop
void loop(){
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

      tflInputTensor->data.f[samples_read * 6 + 0] = ax;
      tflInputTensor->data.f[samples_read * 6 + 1] = ay;
      tflInputTensor->data.f[samples_read * 6 + 2] = az;
      tflInputTensor->data.f[samples_read * 6 + 3] = gx;
      tflInputTensor->data.f[samples_read * 6 + 4] = gy;
      tflInputTensor->data.f[samples_read * 6 + 5] = gz;

      samples_read++;

      if (samples_read == num_samples_per_gesture) {
        end_recording();

        // Run inferencing
        TfLiteStatus invokeStatus = tflInterpreter->Invoke();
        if (invokeStatus != kTfLiteOk) {
          Serial.println("Invoke failed!");
          while (1)
            ;
          return;
        }

        unsigned char max_index = -1;

        float max_value = 0.;
        // Loop through the output tensor values from the model
        for (int i = 0; i < NUM_GESTURES; i++) {
          // Get the predictions score from the output tensor
          const float value = tflOutputTensor->data.f[i];
          if(value > max_value){
            max_value = value;
            max_index = i;
          }
          Serial.print(GESTURES[i]);
          Serial.print(": ");
          Serial.println(value, 6);
        }
        Serial.println("--------------");
        Serial.print("Best guess: ");
        Serial.print(GESTURES[max_index]);
        Serial.println();
        Serial.println();
      }
    }
  }
}
