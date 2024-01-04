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

  // Display BME280 data on OLED
  display.clearDisplay();
  display.setTextSize(1.5);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print(F("Temp: "));
  display.print(bme.readTemperature());
  display.println(F("C"));

  display.setCursor(0, 10);
  display.print(F("Humidity: "));
  display.print(bme.readHumidity());
  display.println(F("%"));
  
  display.display();
  delay(2000); // Wait for 2 seconds

  // Display MPU6050 data on OLED
  display.clearDisplay();
  display.setTextSize(1.5);
  display.setCursor(0,0);
  display.print(F("Gyro X: "));
  display.print(g.gyro.x);

  display.setCursor(0, 10);
  display.print(F("Gyro Y: "));
  display.print(g.gyro.y);

  display.display();
  delay(2000); // Wait for 2 seconds before next loop
}
