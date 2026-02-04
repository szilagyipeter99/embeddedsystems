#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include "sht4x_i2c.h"

void app_main(void) {
  
    sensirion_i2c_hal_init();
    sht4x_init(SHT40_I2C_ADDR_44);
    sht4x_soft_reset();
    sensirion_hal_sleep_usec(10000);
    
    uint32_t serial_number = 0;
    error = sht4x_serial_number(&serial_number);
  
    if (error != NO_ERROR) {
        printf("error executing serial_number(): %i\n", error);
        return error;
    }
  
    printf("serial_number: %u\n", serial_number);
    
    int32_t temperature_milli_degC = 0;
    int32_t humidity_milli_RH = 0;

    error = sht4x_measure_lowest_precision(&temperature_milli_degC, &humidity_milli_RH);
  
    if (error != NO_ERROR) {
        printf("error executing measure_lowest_precision_ticks(): %i\n", error);
        continue;
    }
  
    printf("Temperature milli °C: %i ", temperature_milli_degC);
    printf("Humidity milli percent RH: %i\n", humidity_milli_RH);
}
