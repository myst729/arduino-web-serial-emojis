/*
 * Required Arduino libraries:
 * (1) Simple Web Serial, by Fabian Mohr
 * (2) Adafruit_NeoPixel, by Adafruit
 */

#include <SimpleWebSerial.h>
#include <Adafruit_NeoPixel.h>

#define NEON_PIN 8
#define NEON_NUM 64

#define EVENT_START 1
#define EVENT_DATA  2
#define EVENT_END   3

SimpleWebSerial WebSerial;
Adafruit_NeoPixel neon = Adafruit_NeoPixel(NEON_NUM, NEON_PIN, NEO_GRB + NEO_KHZ800);
uint32_t pixels[NEON_NUM] = {0};
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
  for (uint8_t i = 0; i < NEON_NUM; i++) {
    neon.setPixelColor(i, pixels[i]);
  }
  neon.show();
  mark = 0;
}

void handle(JSONVar data) {
  int event = data[0];
  switch (event) {
    case EVENT_START:
      WebSerial.send("event-from-arduino", EVENT_DATA);
      break;
    case EVENT_DATA:
      store(data);
      WebSerial.send("event-from-arduino", EVENT_DATA);
      break;
    case EVENT_END:
      display();
      WebSerial.send("event-from-arduino", EVENT_END);
      break;
  }
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
