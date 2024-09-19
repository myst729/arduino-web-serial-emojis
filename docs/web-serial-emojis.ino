/*
 * Tested with RP2040 (RaspberryPi Pico, etc.)
 *
 * Required Arduino libraries:
 * - Simple Web Serial, by Fabian Mohr
 * - Adafruit_NeoPixel, by Adafruit
 */

#include <SimpleWebSerial.h>
#include <Adafruit_NeoPixel.h>

#define NEON_PIXEL_PIN 8
#define NEON_PIXEL_NUM 64

#define DATA_EVENT_PREPARE 1
#define DATA_EVENT_READY   2
#define DATA_EVENT_END     3

SimpleWebSerial WebSerial;
Adafruit_NeoPixel neon = Adafruit_NeoPixel(NEON_PIXEL_NUM, NEON_PIXEL_PIN, NEO_GRB + NEO_KHZ800);
uint32_t pixels[NEON_PIXEL_NUM] = {0};
int mark = 0;

void store(JSONVar data) {
  int length = data.length() - 1;
  for (int i = 0; i < length; i++) {
    pixels[mark + i] = data[i + 1];
  }
  mark += length;
}

void display() {
  neon.clear();
  for (uint8_t i = 0; i < NEON_PIXEL_NUM; i++) {
    neon.setPixelColor(i, pixels[i]);
  }
  neon.show();
  mark = 0;
}

void handle(JSONVar data) {
  int event = data[0];
  JSONVar response = JSON.parse("[]");
  switch (event) {
    case DATA_EVENT_PREPARE:
      response[0] = DATA_EVENT_READY;
      break;
    case DATA_EVENT_READY:
      store(data);
      response[0] = DATA_EVENT_READY;
      break;
    case DATA_EVENT_END:
      display();
      response[0] = DATA_EVENT_END;
      break;
    default:
      response[0] = "unknown-event";
      response[1] = "An unknown event was acknowledged.";
      response[2] = JSON.stringify(data);
      break;
  }
  WebSerial.send("event-from-arduino", response);
}

void setup() {
  Serial.begin(9600);
  neon.begin();
  neon.setBrightness(5);
  WebSerial.on("event-to-arduino", handle);
  delay(100);
}

void loop() {
  WebSerial.check();
  delay(5);
}
