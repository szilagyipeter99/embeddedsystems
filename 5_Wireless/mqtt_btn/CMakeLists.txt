idf_component_register(
    SRCS main.c wifi_connector/wifi_connector.c mqtt_connector/mqtt_connector.c
    INCLUDE_DIRS . wifi_connector mqtt_connector
)