#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BME280.h>

// OLED display width and height for a 128x64 display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
// OLED reset pin (not used in this example)
#define OLED_RESET    -1

// Initialize the OLED display using the Wire library
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Initialize the MPU6050 and BME280 sensors
Adafruit_MPU6050 mpu;
Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Change to 0x3D if necessary
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println(F("Failed to find MPU6050 chip"));
    while (1);
  }

  // Initialize BME280
  if (!bme.begin(0x76)) { // Change to 0x77 if necessary
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    while (1);
  }
}

void loop() {
  // Get new sensor events with the readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Determine tilt direction
  String tiltDirection = "";
  if (a.acceleration.x > 1.0) {
    tiltDirection = "Right";
  } else if (a.acceleration.x < -1.0) {
    tiltDirection = "Left";
  } else if (a.acceleration.y > 1.0) {
    tiltDirection = "Up";
  } else if (a.acceleration.y < -1.0) {
    tiltDirection = "Down";
  } else {
    tiltDirection = "Flat";
  }

  // Read and round temperature and humidity
  float temperature = round(bme.readTemperature() * 10) / 10.0;
  float humidity = round(bme.readHumidity() * 10) / 10.0;

  // Centering the text on the display
  display.clearDisplay();
  display.setTextSize(2); // Larger text size
  display.setTextColor(SSD1306_WHITE);

  int16_t x1, y1;
  uint16_t w, h;

  // Center and display temperature
  String tempString = "Temp:" + String(temperature, 1) + "C";
  display.getTextBounds(tempString, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 0);
  display.print(tempString);

  // Center and display humidity
  String humString = "Hum:" + String(humidity, 1) + "%";
  display.getTextBounds(humString, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 16 + 4); // Additional padding
  display.print(humString);

  // Center and display tilt direction
  display.getTextBounds(tiltDirection, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 32 + 8); // Additional padding
  display.print(tiltDirection);
  
  display.display();
  delay(1000); // Update every second
}
