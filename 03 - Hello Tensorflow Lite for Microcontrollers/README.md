# Hello TensorFlow Lite for Microcontroller
In this exercise we will create the "Hello World" of TinyML. Designed by Pete Warden as an introduction to TensorFlow Lite for Microcontrollers. While this model is not a particularly useful model (a sine wave can easily be produced with a normal sin function) it covers a lot of what we've gone over in class.

The idea is to train a simple ANN to generate a sine wave with *Regression*. 

## Installation
Start by installing TensorFlow Lite For Microcontrollers
1. Open the Arduino IDE
1. Make sure you have the board library installed, if not go back and follow the instructions in `00 - Setup/README.md` in this repo.
1. Go to `Tools->Library Manager`
1. Search for TensorFlow and install Arduino_TensorFlowLite published by Pete Warden.
1. Wait for the installation to complete

## Run the example
To make sure everything is working, let's run the pre-build model.
1. Plugin your Arduino Nano 33 BLE Sense
1. Select the right port under `Tools->Port...`
1. Make sure you have the Arduino Nano 33 BLE selected under `Tools->Board` 
1. Go to `File->Examples->Arduino_TensorflowLite->hello_world`
1. Hit the arrow button to build and upload your sketch
1. Once built, the orange LED should be pulsing on your board.
1. Open up the Serial Plotter by going to `Tools->Serial Plotter`
1. You should see a sine wave being printed in the plotter

## Train your own model
1. Save the hello world example so you can edit it.
1. Go to [this](https://colab.research.google.com/gist/rikard-io/0474a1f7ecd688a428d166bc0b17a98b/train_hello_world_model.ipynb) colab and follow the instructions. Try to relate the steps to what we've gone over in class.
1. Once you have your model, replace the **values** in the file model.cpp file with the **values** of the one produced in the colab. *If you just copy and paste the file, you'll end up with wrong variable types.*
1. Test the new model by building and uploading the sketch again
1. Confirm that the Serial Plotter prints the sine wave.
1. Write down any questions you have and we'll discuss it in class next week.
