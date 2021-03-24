/* This is a BLE and WS2812s LED on ESP32 by Reza Hadipour */
/* 2021-03-24 */

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include <Adafruit_NeoPixel.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "19B10000-E8F2-537E-4F6C-D104768A1214"

#define PIN 13
Adafruit_NeoPixel strip(16, PIN, NEO_GRB + NEO_KHZ800);
uint8_t rgb_values[3];

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      int xxx = 1;

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
//        xxx = value;
        Serial.print("Original Value is: ");
        Serial.println(value.c_str());
        for (int i = 0; i < value.length(); i++){
          Serial.print(value[i]);
        }
        xxx = String(value.c_str()).toInt();
       
    switch (xxx)
    {
    case 1:
        colorWipe(strip.Color(255, 0, 0), 20); // Red
        break;
    case 2:
        colorWipe(strip.Color(0, 255, 0), 20); // Green
        Serial.print("CASE 1");
        break;
    case 3:
        colorWipe(strip.Color(0, 0, 255), 20); // Blue
        break;
    case 4:
        theaterChase(strip.Color(255, 0, 0), 20); // Red
        break;
    case 5:
        theaterChase(strip.Color(0, 255, 0), 20); // Green
        break;
    case 6:
        theaterChase(strip.Color(255, 0, 255), 20); // Cyan
        break;
    case 7:
        rainbow(10);
        break;
    case 8:
        theaterChaseRainbow(20);
        break;
    }
    
        Serial.println();
        Serial.println("*********");
        strip.clear();
        strip.show();
        
      }
    }

void colorWipe(uint32_t color, int wait)
{
    for (int i = 0; i < strip.numPixels(); i++)
    {                                  // For each pixel in strip...
        strip.setPixelColor(i, color); //  Set pixel's color (in RAM)
        strip.show();                  //  Update strip to match
        delay(wait);                   //  Pause for a moment
    }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait)
{
    for (int a = 0; a < 10; a++)
    { // Repeat 10 times...
        for (int b = 0; b < 3; b++)
        {                  //  'b' counts from 0 to 2...
            strip.clear(); //   Set all pixels in RAM to 0 (off)
            // 'c' counts up from 'b' to end of strip in steps of 3...
            for (int c = b; c < strip.numPixels(); c += 3)
            {
                strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
            }
            strip.show(); // Update strip with new contents
            delay(wait);  // Pause for a moment
        }
    }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait)
{
    // Hue of first pixel runs 5 complete loops through the color wheel.
    // Color wheel has a range of 65536 but it's OK if we roll over, so
    // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
    // means we'll make 5*65536/256 = 1280 passes through this outer loop:
    for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256)
    {
        for (int i = 0; i < strip.numPixels(); i++)
        { // For each pixel in strip...
            // Offset pixel hue by an amount to make one full revolution of the
            // color wheel (range of 65536) along the length of the strip
            // (strip.numPixels() steps):
            int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
            // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
            // optionally add saturation and value (brightness) (each 0 to 255).
            // Here we're using just the single-argument hue variant. The result
            // is passed through strip.gamma32() to provide 'truer' colors
            // before assigning to each pixel:
            strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
        }
        strip.show(); // Update strip with new contents
        delay(wait);  // Pause for a moment
    }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait)
{
    int firstPixelHue = 0; // First pixel starts at red (hue 0)
    for (int a = 0; a < 30; a++)
    { // Repeat 30 times...
        for (int b = 0; b < 3; b++)
        {                  //  'b' counts from 0 to 2...
            strip.clear(); //   Set all pixels in RAM to 0 (off)
            // 'c' counts up from 'b' to end of strip in increments of 3...
            for (int c = b; c < strip.numPixels(); c += 3)
            {
                // hue of pixel 'c' is offset by an amount to make one full
                // revolution of the color wheel (range 65536) along the length
                // of the strip (strip.numPixels() steps):
                int hue = firstPixelHue + c * 65536L / strip.numPixels();
                uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
                strip.setPixelColor(c, color);                       // Set pixel 'c' to value 'color'
            }
            strip.show();                // Update strip with new contents
            delay(wait);                 // Pause for a moment
            firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
        }
    }
}
};



class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("*** onConnect ***");
      colorWipe(strip.Color(0, 0, 128), 40);
      delay(500);
      strip.clear();
      strip.show();
    };

    void onDisconnect(BLEServer* pServer) {
      Serial.println("*** onDisconnect ***");
      colorWipe(strip.Color(128, 0, 0), 35);
      delay(500);
      strip.clear();
      strip.show();
    }

    void colorWipe(uint32_t color, int wait)
{
    for (int i = 0; i < strip.numPixels(); i++)
    {                                  // For each pixel in strip...
        strip.setPixelColor(i, color); //  Set pixel's color (in RAM)
        strip.show();                  //  Update strip to match
        delay(wait);                   //  Pause for a moment
    }
}
};


void setup() {
  Serial.begin(115200);

  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");

  BLEDevice::init("MyESP32_Notofy BOX");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pServer->setCallbacks(new MyServerCallbacks());
  
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World LED");
  //pCharacteristic->notify();
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();  // Turn OFF all pixels ASAP

  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);

  strip.setPixelColor(0,strip.Color(0,0,68));
  strip.setPixelColor(8,strip.Color(0,0,68));
  strip.show();
  delay(400);
  strip.clear();
  strip.show();
  delay(100);
  strip.setPixelColor(4,strip.Color(0,0,68));
  strip.setPixelColor(12,strip.Color(0,0,68));
  strip.show();
  delay(400);
  strip.clear();
  strip.show();
  delay(100);
  strip.setPixelColor(0,strip.Color(0,0,128));
  strip.setPixelColor(8,strip.Color(0,0,128));
  strip.setPixelColor(4,strip.Color(0,0,128));
  strip.setPixelColor(12,strip.Color(0,0,128));
  strip.show();
  delay(700);
  strip.clear();
  strip.show();
    
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}



