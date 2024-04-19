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

  plantSet();

  void check_pump();
  void check();
  void check_digital();
};

#endif  // __PLANTS_H__