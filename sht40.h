#ifndef SHT40_H
#define SHT40_H

typedef enum {
    HIGH_HEAT_MEASURE_1S      = 0x39,
    HIGH_HEAT_MEASURE_100MS   = 0x32,
    MEDIUM_HEAT_MEASURE_1S    = 0x2F,
    MEDIUM_HEAT_MEASURE_100MS = 0x24,
    LOW_HEAT_MEASURE_1S       = 0x1E,
    LOW_HEAT_MEASURE_100MS    = 0x15
} heater_status_t;

int sht40_heater(heater_status_t heater);
int sht40_read(float *temperature, float *humidity);
uint8_t* sht40_serial(void);
int sht40_reset(void);

#endif