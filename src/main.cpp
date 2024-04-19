#include <Arduino.h>

#include "garden.h"
#include "print_status.h"

#define CHECK_PERIOD 600  // 600 = 10min
#define ONE_SECOND 1000
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
unsigned long time_init;

void check_status() {
  //
  // Serial.println("1 sec called");
  // Serial.print(min10);
  // check water level in the container
  if (!digitalRead(WATER_SENSOR_PIN)) {
    disable_system = false;
    // Serial.println(" Water in the deposit");
  } else {
    disable_system = true;
    // Serial.println(" No water in the deposit");
  }

  garden.checkPump();

  min10++;
}

void setup() {
  Serial.begin(115200);

  disable_system = false;
  pinMode(WATER_SENSOR_PIN, INPUT_PULLUP);
  // Analog pin, digital pin to relay, humidity threshold, seconds on
  garden.addPlant(plant1, SENSOR0, POWER_SENSOR0, RELAY1, 30, 10);
  garden.addPlant(plant2, SENSOR1, POWER_SENSOR1, RELAY2, 30, 10);
  garden.addPlant(plant3, SENSOR2, POWER_SENSOR2, RELAY3, 30, 10);
  // garden.addPlant(plant4, A19, POWER_SENSOR3, RELAY4, 30, 10);

  init_display();

  check_status();
  garden.checkPlants();
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
      // p = garden.getStatus(plant4);
      // print_status(3, plant4, p.humidity, p.pump_status);
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
