#ifndef __PLANTS_H__
#define __PLANTS_H_

#include <Arduino.h>

#include "moisture.h"
#include "pump.h"

typedef struct {
  bool pump_status;
  int humidity;
} plant_status_t;

class plantSet : public Moisture, public Pump {
 public:
  // moisture humidity;
  // Relays pump;
  int duration;
  int threshold;
  int humidity;
  unsigned long time_started;
  String name;

  plantSet* next;

  plantSet() {
    this->duration = 0;
    this->threshold = 0;
    this->humidity = 0;
  }

  void check_pump() {
    // Serial.printf("Status %d\n",status());
    if (Pump::status()) {
      // pump is on, check for time elapsed
      // and turn off if time has passed
      Serial.println(millis() - time_started);
      if ((millis() - time_started) >= (unsigned long)(duration)) {
        turnOff();
        time_started = 0;
      }
    }
  }

  void check() {
    // Check the humidity to turn it on if required
    this->humidity = Moisture::read();
    if (!Pump::status() && (this->humidity < threshold)) {
      turnOn();
      time_started = millis();
      // Serial.printf("moisture below threshold %d\n", time_started);
      Serial.println("moisture below threshold ");
    }
  }

  void check_digital() {
    if (Moisture::status()) {
      turnOn();
      time_started = millis();
      // Serial.printf("moisture below threshold %d\n", time_started);
      Serial.println("moisture below threshold ");
    }
  }
};

class Garden {
 private:
  plantSet* root;
  bool any_pump_is_on;

 public:
  Garden();

  void addPlant(String name, int pin, int v_pin, int relay, int threshold,
                int duration);
  void checkPlants();
  void checkPump();
  plant_status_t getStatus(String name);
};

#endif  // __PLANTS_H__