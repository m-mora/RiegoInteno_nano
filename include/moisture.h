#ifndef __MOISTURE_H__
#define __ MOISTURE_H__

#include <Arduino.h>
#include <stdint.h>

// const int dry_value = 2700;
// const int water_value = 1040;
const int dry_value = 1000;
const int water_value = 223;

class Moisture {
 private:
  int pin;
  int v_pin;

 public:
  Moisture(int pin, int v_pip);
  Moisture();
  ~Moisture();
  int read();
  bool status();
  void init(int pin, int v_pin);
};

#endif  //__MOISTURE_H__
