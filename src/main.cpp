#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "plants.h"

#define CHECK_PERIOD  600  // 600 = 10min
#define ONE_SECOND    1000
// pins to connect  moisture sensors
#define SENSOR0 A2
#define SENSOR1 A1
#define SENSOR2 A0
#define SENSOR3 
#define POWER_SENSOR0 9
#define POWER_SENSOR1 8
#define POWER_SENSOR2 7
#define POWER_SENSOR3 
// #define MUX_ENABLE 33
#define RELAY1 2
#define RELAY2 3
#define RELAY3 4
#define RELAY4 
#define WATER_SENSOR_PIN 12

uint16_t min10 = 0;
bool disable_system;
// Create teh objects
Garden garden;
String plant1 = "Plant1";
String plant2 = "Plant2";
String plant3 = "Plant3";
String plant4 = "Plant4";
int plant1_id = 1;
int plant2_id = 2;
int plant3_id = 3;
int plant4_id = 4;
unsigned long time_init;

// Liquid crystal
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 20, 4);

void check_status() {
  //
  // Serial.println("1 sec called");
  Serial.print(min10);
  // check water level in the container
  if (!digitalRead(WATER_SENSOR_PIN)) {
    disable_system = false;
    Serial.println(" Water in the deposit");
  } else {
    disable_system = true;
    Serial.println(" No water in the deposit");
  }

  garden.checkPump();

  min10++;
}

byte pump_on[] = {0x04, 0x0E, 0x15, 0x04, 0x04, 0x04, 0x04, 0x04};

byte pump_off[] = {0x00, 0x00, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x00};

void print_checking() {
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.printstr("Checking plants");
  lcd.setCursor(3, 2);
  lcd.print("In progress...");
}

void print_status(int pos, String name, int humid, bool status) {
  if (pos == 0) {
    lcd.clear();
  }
  lcd.blink_off();
  lcd.setCursor(0, pos);
  lcd.print(name);
  lcd.setCursor(8, pos);
  lcd.print(humid);
  lcd.print("%");
  lcd.setCursor(14, pos);
  lcd.print("Pump ");
  if (status) {
    lcd.write(0);
  } else {
    lcd.write(1);
  }
}

void print_no_water() {
  lcd.clear();
  lcd.setCursor(9, 1);
  lcd.blink_on();
  lcd.print("NO");
  lcd.setCursor(5, 2);
  lcd.print("** WATER **");
};

void setup() {
  Serial.begin(115200);

  disable_system = false;
  pinMode(WATER_SENSOR_PIN, INPUT_PULLUP);
  // Analog pin, digital pin to relay, humidity threshold, seconds on
  garden.addPlant(plant1, A0, RELAY1, 30, 10);
  garden.addPlant(plant2, A3, RELAY2, 30, 10);
  garden.addPlant(plant3, A7, RELAY3, 30, 10);
  //garden.addPlant(plant4, A19, RELAY4, 30, 10);

  // Wire.begin();
  // scan_i2c();
  // Liquid Crystal
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, pump_on);
  lcd.createChar(1, pump_off);
  check_status();
}

void loop() {
  // one seconf functions called here
  if ((millis() - time_init) >= ONE_SECOND) {
    check_status();
    time_init = millis();
    if (disable_system) {
      print_no_water();
    } else {
      plant_status_t p;
      p = garden.getStatus(plant1);
      print_status(0, plant1, p.humidity, p.pump_status);
      p = garden.getStatus(plant2);
      print_status(1, plant2, p.humidity, p.pump_status);
      p = garden.getStatus(plant3);
      print_status(2, plant3, p.humidity, p.pump_status);
      p = garden.getStatus(plant4);
      print_status(3, plant4, p.humidity, p.pump_status);
    }
  }
  // ten minutes function called here
  if ((min10 >= CHECK_PERIOD) && !disable_system) {
    min10 = 0;
    Serial.println("Check plants");
    // check humidity and turn pump if required.
    print_checking();
    garden.checkPlants();
  }

  delay(500);
}
