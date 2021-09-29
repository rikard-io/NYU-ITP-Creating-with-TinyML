# Setup Arduino Dev Environment and Arduino 33 BLE Nano

## Install Arduino IDE

1. Go to [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software) and download the Arduino IDE for your OS
1. Install the package. Further instructions here: 
    * [windows](https://www.arduino.cc/en/guide/windows)
    * [MacOS](https://www.arduino.cc/en/Guide/macOS)
    * [Linux](https://www.arduino.cc/en/Guide/Linux)

## Install the Arduino BLE 33 Board
1. Open the Arduino IDE
1. Go to `Tools -> Board -> Board Manager`
1. Search for "Arduino Mbed OS Nano Boards"
1. Select "Arduino Mbed OS Nano Boards" and click install

## Test it out
1. Plugin your Nano 33 BLE Sense board into your computer.
1. Go to `Tools -> Board -> Arduino Mbed OS Nano Boards -> Arduino Nano 33 BLE`
1. Go to `Tools -> Port` and select the port with "Arduino Nano 33 BLE" in it's name. On MacOS it shows up as `/dev/cu.usbmodem14012 (Arduino Nano 33 BLE)` for me. 
    * If you're having problem seeing what port to select, try unplugging the board, look at the Port list, then plug the board back in to see what port shows up
1. Open 00_HelloWorld/00_HelloWorld.ino in this folder
1. Click the right pointing arrow in the upper left corner to build and upload the sketch.
1. Once the sketch is uploaded, open the Serial monitor, either by going to `Tools -> Serial Monitor` or by pressing the magnifier in the upper right corner.
1. Once the sketch is running and the Serial Monitor is connected, there should be a line saying "Hello World!" in the console.
1. Try typing something in the input field. The Arduino should echo whatever you wrote, prefixed with "echo: "
