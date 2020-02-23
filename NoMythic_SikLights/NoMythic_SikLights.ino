/*******************************************************
 * NoMythic SikLights Neopixels controlled over I2C    *
 * Listen on I2C for light animation commands.         *
 *******************************************************/

#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

/*******************************************************
 * NeoPixel Config                                     *
 *******************************************************/
 
// The pin the NeoPixels are attached to
// These are analog input pins, but are used due to location on the board
#define PIN1       14
#define PIN2       15

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS  120

// How bright are the neo pixels? (0-100)
#define BRIGHTNESS 100

// Delay defines the blink rate in ms
#define DELAY      400

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(NUMPIXELS, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800);

// Testing different purples
// pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
uint32_t purple = pixels1.Color(102, 000, 204); // Purple
uint32_t color2 = pixels1.Color(161, 000, 255);
uint32_t color3 = pixels1.Color(150, 025, 148);
uint32_t color4 = pixels1.Color(91, 36, 98);
uint32_t orange = pixels1.Color(255, 69, 0);
uint32_t red = pixels1.Color(255, 0, 0);

/*******************************************************
 * I2C Config                                          *
 *******************************************************/

//The I2C device number. Must use same number in sender
const byte DEVICE_NO = 4;
// Status pin - a led - onboard is 13 - pulses for each command
const byte STATUS_PIN = 13;

void setup() {
  Serial.begin(9600);
  Serial.println("NoMythic SikLights");
  pinMode(13, OUTPUT);

  // Strand 1
  pixels1.begin(); // This initializes the NeoPixel library.
  pixels1.setBrightness(BRIGHTNESS);

  // Strand 2
  pixels2.begin(); // This initializes the NeoPixel library.
  pixels2.setBrightness(BRIGHTNESS);

  Wire.begin(DEVICE_NO);
  // when an I2C event happens, process it
  Wire.onReceive(handleEvent);
  Serial.println("I2C Ready");

  setSolidColor1(purple);
  setSolidColor2(purple);

  Serial.println("Pixels On");
}

/*******************************************************
 * Every time we hear an I2C event for our device,     *
 * handle the command here                             *
 *******************************************************/
void handleEvent(int howMany) {
  digitalWrite(STATUS_PIN, HIGH);
  while(Wire.available()) {
    byte command = Wire.read();
    Serial.print("Command: ");
    Serial.println(command);
    // we have predefined commands that both send and receive understand
    switch(command) {
      case 1: // turn off command
        turnOff1();
        turnOff2();
        break;
      case 2: // turn on command
        setSolidColor1(purple);
        setSolidColor2(purple);
        break;
      case 3: // last 30 sec command
        last30Seconds1();
        last30Seconds2();
        break;
      case 4: // limelight lined up
        blinkRed();
    }
  }
  digitalWrite(STATUS_PIN, LOW);
}

// Nothing happening right now
void loop() {
  delay(DELAY);
}

// Set all led on strand 1 to one color
void setSolidColor1(uint32_t color) {
  for(int i=0;i<NUMPIXELS;i++){
    pixels1.setPixelColor(i, color);
  }
  pixels1.show();
}

// Set all led on strand 2 to one color
void setSolidColor2(uint32_t color) {
  for(int i=0;i<NUMPIXELS;i++){
    pixels2.setPixelColor(i, color);
  }
  pixels2.show();
}

// Turn off all lights on strand 1
void turnOff1() {
  for(int i=0;i<NUMPIXELS;i++){
    pixels1.setPixelColor(i, 0);
  }
  pixels1.show();
}

// Turn off all lights on strand 2
void turnOff2() {
  for(int i=0;i<NUMPIXELS;i++){
    pixels2.setPixelColor(i, 0);
  }
  pixels2.show();
}

void last30Seconds1() {
  int start = NUMPIXELS / 3;
  int finish = start * 2;
  for(int i = start;i < finish;i++){
    pixels1.setPixelColor(i, orange);
  }
  pixels1.show();
}

void last30Seconds2() {
  int start = NUMPIXELS / 3;
  int finish = start * 2;
  for(int i = start;i < finish;i++){
    pixels2.setPixelColor(i, orange);
  }
  pixels2.show();
}

void blinkRed() {
  setSolidColor1(red);
  setSolidColor2(red);
  delay(300);
  // setPurple();
}
