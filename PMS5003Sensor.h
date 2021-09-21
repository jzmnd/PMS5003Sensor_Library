/*
  PMS5003Sensor.h
  PMS 5003 Sensor Library
*/

#ifndef PMS5003_SENSOR_H_
#define PMS5003_SENSOR_H_

#define DEBUG

#include <Arduino.h>

struct PMS5003data
{
  // frame length = 2 * 13 + 2(data + check bytes)
  uint16_t framelen;
  // standard particles (CF=1) ug/m3 
  uint16_t pm1_0_standard, pm2_5_standard, pm10_0_standard;
  // atmospheric environment ug/m3
  uint16_t pm1_0_env, pm2_5_env, pm10_0_env;
  // numbers of particles per 0.1 L
  uint16_t particles_0_3um, particles_0_5um, particles_1_0um, particles_2_5um, particles_5_0um, particles_10_0um;
  uint16_t unused;
  // check sum
  uint16_t checksum;
};

template <class T>
class PMS5003Sensor
{
  public:
    explicit PMS5003Sensor(T&);
    bool read();
    String pm1_0str() { return String(pmsData.pm1_0_standard); }
    String pm2_5str() { return String(pmsData.pm2_5_standard); }
    String pm10_0str() { return String(pmsData.pm10_0_standard); }
    String pm1_0envstr() { return String(pmsData.pm1_0_env); }
    String pm2_5envstr() { return String(pmsData.pm2_5_env); }
    String pm10_0envstr() { return String(pmsData.pm10_0_env); }
    String particles_0_3um_str() { return String(pmsData.particles_0_3um); }
    String particles_0_5um_str() { return String(pmsData.particles_0_5um); }
    String particles_1_0um_str() { return String(pmsData.particles_1_0um); }
    String particles_2_5um_str() { return String(pmsData.particles_2_5um); }
    String particles_5_0um_str() { return String(pmsData.particles_5_0um); }
    String particles_10_0um_str() { return String(pmsData.particles_10_0um); }
    void print_cu_std();
    void print_cu_env();
    void print_particles();

  private:
    T* stream;
    PMS5003data pmsData;
};

#endif
