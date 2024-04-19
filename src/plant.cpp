#include "plants.h"

plantSet::plantSet() {
  this->duration = 0;
  this->threshold = 0;
  this->humidity = 0;
}

void plantSet::check_pump() {
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

void plantSet::check() {
  // Check the humidity to turn it on if required
  this->humidity = Moisture::read();
  if (!Pump::status() && (this->humidity < threshold)) {
    turnOn();
    time_started = millis();
    // Serial.printf("moisture below threshold %d\n", time_started);
    Serial.println("moisture below threshold ");
  }
}

void plantSet::check_digital() {
  if (Moisture::status()) {
    turnOn();
    time_started = millis();
    // Serial.printf("moisture below threshold %d\n", time_started);
    Serial.println("moisture below threshold ");
  }
}
