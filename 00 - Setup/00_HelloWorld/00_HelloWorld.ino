// Arduino Nano 33 BLE - Hello World
//
// The Nano 33 BLE (and many other Arduino Board) emulates
// a UART port for Serial communication over the USB cable.
// The global Serial is reserved for this port
//

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

void setup()
{
  // Setup pins for RGB LED
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // Show yellow until connected
  rgbLedRed();
  
  // Open serial communications with a baud rate of 9600
  Serial.begin(9600);

  // Serial is false until a connection has been established
  // So this is a way to wait for that to happen
  while (!Serial);

  // Show green when we connect
  rgbLedGreen();
  
  Serial.println("Hello World!");
}

void loop()
{
  if (Serial.available()){
    // Read string sent over the Serial port
    String s = Serial.readString();
    Serial.print("echo: " + s);
  }
}
