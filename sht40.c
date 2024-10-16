#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#include "sht40.h"

LOG_MODULE_REGISTER(sht40, CONFIG_SIDEWALK_LOG_LEVEL);

static struct i2c_dt_spec i2c_sht40 = I2C_DT_SPEC_GET(DT_NODELABEL(sht40));

#define HIGH_PRECISE_MEASURE 0xFD
#define MEDIUM_PRECISE_MEASURE 0xF6
#define LOW_PRECISE_MEASURE 0xE0
#define READ_SERIAL 0x89
#define SOFT_RESET 0x94

uint8_t rx_buf[10], tx_buf[10];

int sht40_heater(heater_status_t heater)
{
    int ret = 0;
    
    tx_buf[0] = heater;

    ret = i2c_write_dt(&i2c_sht40, tx_buf, 1);
    if (ret != 0) {
        LOG_ERR("i2c write dt set heater error: %d", ret);
        return ret;
    }

    return ret;
}

int sht40_read(float *temperature, float *humidity)
{
    int ret = 0;

    tx_buf[0] = HIGH_PRECISE_MEASURE;

    ret = i2c_write_dt(&i2c_sht40, tx_buf, 1);
    if (ret != 0) {
        LOG_ERR("i2c write dt read value error: %d", ret);
        return ret;
    }

    k_msleep(10);

    ret = i2c_read_dt(&i2c_sht40, rx_buf, 6);
    if (ret != 0) {
        LOG_ERR("i2c read dt read value error: %d", ret);
        return ret;
    }

    /* c temperature calculate */
    *temperature = (((rx_buf[0] * 256 + rx_buf[1]) * 175) / 65535.0) - 45;
    /* f temperature calculate */
    // rx_buf[1] = rx_buf[0] * 1.8 + 32;
    /* humidity calculate */
    *humidity = (((rx_buf[3] * 256 + rx_buf[4]) * 125) / 65535.0) - 6;

    return ret;
}

uint8_t* sht40_serial(void)
{
    int ret = 0;

    tx_buf[0] = READ_SERIAL;

    ret = i2c_write_dt(&i2c_sht40, tx_buf, 1);
    if (ret != 0) {
        LOG_ERR("i2c write dt read serial error: %d", ret);
        return NULL;
    }

    k_msleep(1);

    ret = i2c_read_dt(&i2c_sht40, rx_buf, 6);
    if (ret != 0) {
        LOG_ERR("i2c read dt read serial error: %d", ret);
        return NULL;
    }

    return rx_buf;
}

int sht40_reset(void)
{
    int ret = 0;

    tx_buf[0] = SOFT_RESET;

    ret = i2c_write_dt(&i2c_sht40, tx_buf, 1);
    if (ret != 0) {
        LOG_ERR("i2c write dt soft reset error: %d", ret);
        return ret;
    }

    return ret;
}
