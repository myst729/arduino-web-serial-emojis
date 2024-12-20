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

#define EVENT_STATE_READY  1
#define EVENT_STATE_CHUNK  2
#define EVENT_STATE_FINISH 3

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
    case EVENT_STATE_READY:
      response[0] = EVENT_STATE_CHUNK;
      break;
    case EVENT_STATE_CHUNK:
      store(data);
      response[0] = EVENT_STATE_CHUNK;
      break;
    case EVENT_STATE_FINISH:
      display();
      response[0] = EVENT_STATE_FINISH;
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
