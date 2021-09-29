# kNN to classify Colors on Arduino
This is a two step experiment. First we'll make sure we can read the sensor and understand the output. Then we'll move on to classifying it.

## Install the APDS9960 library
The APDS9960 is the Light, Proximity and Gesture sensor

1. Open the Arduino IDE
1. Go to `Tool -> Library Manager`
1. Search for Arduino_APDS9960
1. Install Arduino_APDS9960

## Run the Serial Logging Example
1. Open 01a-Read-Color-Sensor/01a-Read-Color-Sensor.ino
1. Make sure you've selected the correct board and port under `Tools`
1. Click the run button – right arrow in the top left corner – to run and upload the sketch
1. Open the serial monitor

### Modify the sketch (Optional)
1. Use the onboard RGB LED to show the color detected. 
    * There might be a risk that the color affects the reading, causing a feedback loop, so turn the LED off or set to white while doing the reading.
1. Create a sketch in [Processing](https://processing.org/download) to show the color. 
    * A tutorial can be found [here](https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all). But just opening Processing and going to `File -> Examples... -> Libraries -> Serial -> SimpleRead` should give you a good start.
    * One way to do it is by sending the data as a json array and using [JSONArray](https://processing.org/reference/JSONArray.html) to parse it.
    * This could be done with **P5js**, but since P5js runs in a web browser, we don't have access to the serial port directly. This could be solved by using BLE (WebBLE API) or by using a middle-man app like what's described [here](https://medium.com/@yyyyyyyuan/tutorial-serial-communication-with-arduino-and-p5-js-cd39b3ac10ce)
    * You probably want to increase the rate of sending the data from once every second.
1. Use the [proximity sensor](https://www.arduino.cc/en/Reference/ArduinoAPDS9960readProximity) to only read the color when something is within a certain distance.
1. If you done any of the above. You probably noticed the readings aren't super great and might be very low. Why is that? What can you do about it?

## Run the kNN Example

### Install the libraries
By now you should have the `Arduino_APDS9960` library ready for reading the sensor values, and should know how to install new libraries. Go ahead and install the `Arduino_KNN` library.