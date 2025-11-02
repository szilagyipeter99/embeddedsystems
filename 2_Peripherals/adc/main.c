#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include <unistd.h>

#define MEAS_PIN ADC_CHANNEL_4 // GPIO 4

static const char *TAG = "Main";

// These variables will always hold the latest values
static int raw_meas;
static int cali_voltage;

void app_main(void) {

	/*

	- This example installs an ADC instance for oneshot mode
	- Info about continuous mode:
	  https://docs.espressif.com/projects/esp-idf/en/v5.5.1/esp32c6/api-reference/peripherals/adc_continuous.html#driver-concepts

	*/

	// Allocate a handle for the ADC instance
	adc_oneshot_unit_handle_t adc_handle;

	// Unit configuration for the initialization process
	adc_oneshot_unit_init_cfg_t init_config = {
		.unit_id = ADC_UNIT_1,
		.clk_src = ADC_DIGI_CLK_SRC_DEFAULT,
		.ulp_mode = ADC_ULP_MODE_DISABLE,
	};

	// Channel configuration
	adc_oneshot_chan_cfg_t input_config = {
		.atten = ADC_ATTEN_DB_12,	 // 12 dB attenuation (divide signal by ~4)
		.bitwidth = ADC_BITWIDTH_12, // 12-bit resolution
	};

	adc_oneshot_new_unit(&init_config, &adc_handle);
	adc_oneshot_config_channel(adc_handle, MEAS_PIN, &input_config);

	/*

	- It is possible to convert the raw ADC measurement value to the real
	  measured voltage. By design, the ADC reference voltage for ESP32C6 is
	  1100 mV, but in reality it ranges from 1000 mV to 1200 mV. Using the
	  calibration driver the output results will be more accurate.

	- During production, Espressif measures the ADC response of each chip
	  against known reference voltages. These measurements are used to determine
	  correction parameters, which are then stored in a special OTP memory.

	- ESP32C6 supports the 'ADC_CALI_SCHEME_VER_CURVE_FITTING' scheme.

	- The ADC is sensitive to noise, a bypass capacitor (like a 100 nF ceramic)
	  may need to be connected to the ADC input to minimize noise. To further
	  mitigate the effects of noise multisampling may be used.

	*/

	// Calibration handle
	adc_cali_handle_t cali_handle = NULL;

	// Configure a Curve Fitting calibration scheme
	adc_cali_curve_fitting_config_t cali_config = {
		.unit_id = ADC_UNIT_1,
		.chan = MEAS_PIN,
		.atten = ADC_ATTEN_DB_12,
		.bitwidth = ADC_BITWIDTH_12,
	};

	adc_cali_create_scheme_curve_fitting(&cali_config, &cali_handle);

	while (true) {
		// Perform a measurement and write it into 'raw_meas'
		adc_oneshot_read(adc_handle, MEAS_PIN, &raw_meas);
		// Convert the result to a voltage and write it into 'cali_voltage'
		adc_cali_raw_to_voltage(cali_handle, raw_meas, &cali_voltage);
		ESP_LOGI(TAG, "Voltage: %d mV (Raw: %d)", cali_voltage, raw_meas);
		sleep(1);
	}
}
