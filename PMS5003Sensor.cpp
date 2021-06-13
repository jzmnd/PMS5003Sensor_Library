/*
  PMS5003Sensor.cpp
  PMS 5003 Sensor Library
*/

#include <HardwareSerial.h>
#include "PMS5003Sensor.h"

PMS5003Sensor::PMS5003Sensor(Stream& s)
  : pmsData()
{
  this->stream = &s;
}

bool PMS5003Sensor::read()
{
  if (! stream->available()) {
    return false;
  }

  // read a byte at a time until we get to the special '0x42' start-byte
  if (stream->peek() != 0x42) {
    stream->read();
    return false;
  }

  // now read all 32 bytes
  if (stream->available() < 32) {
    return false;
  }
    
  uint8_t buffer[32];
  uint16_t sum = 0;
  stream->readBytes(buffer, 32);
 
  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }

  #ifdef DEBUG
  for (uint8_t i=0; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(",");
  }
  Serial.println();
  #endif

  // the data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }

  // copy to struct
  memcpy((void *)&pmsData, (void *)buffer_u16, 30);
 
  if (sum != pmsData.checksum) {
    Serial.println("Checksum failure");
    return false;
  }
  // success
  return true;
}

String PMS5003Sensor::pm1_0str()
{
  return String(pmsData.pm1_0_standard);
}

String PMS5003Sensor::pm2_5str()
{
  return String(pmsData.pm2_5_standard);
}

String PMS5003Sensor::pm10_0str()
{
  return String(pmsData.pm10_0_standard);
}

void PMS5003Sensor::print_cu_std()
{
  Serial.println("Concentration Units (standard)");
  Serial.print("PM 1.0: "); Serial.print(pmsData.pm1_0_standard);
  Serial.print("\t\tPM 2.5: "); Serial.print(pmsData.pm2_5_standard);
  Serial.print("\t\tPM 10: "); Serial.println(pmsData.pm10_0_standard);
}

void PMS5003Sensor::print_cu_env()
{
  Serial.println("Concentration Units (environmental)");
  Serial.print("PM 1.0: "); Serial.print(pmsData.pm1_0_env);
  Serial.print("\t\tPM 2.5: "); Serial.print(pmsData.pm2_5_env);
  Serial.print("\t\tPM 10: "); Serial.println(pmsData.pm10_0_env);
}

void PMS5003Sensor::print_particles()
{
  Serial.print("Particles >  0.3um/0.1L: "); Serial.println(pmsData.particles_0_3um);
  Serial.print("Particles >  0.5um/0.1L: "); Serial.println(pmsData.particles_0_5um);
  Serial.print("Particles >  1.0um/0.1L: "); Serial.println(pmsData.particles_1_0um);
  Serial.print("Particles >  2.5um/0.1L: "); Serial.println(pmsData.particles_2_5um);
  Serial.print("Particles >  5.0um/0.1L: "); Serial.println(pmsData.particles_5_0um);
  Serial.print("Particles > 10.0um/0.1L: "); Serial.println(pmsData.particles_10_0um);
}
