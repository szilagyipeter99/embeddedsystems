idf_component_register(
    SRCS main.c sht4x_driver/sensirion_common.c sht4x_driver/sensirion_i2c.c sht4x_driver/sensirion_i2c_hal.c sht4x_driver/sht4x_i2c.c 
    INCLUDE_DIRS sht4x_driver
)
