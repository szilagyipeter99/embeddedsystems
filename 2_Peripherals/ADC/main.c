#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include <stdbool.h>
#include <unistd.h>

static int adc_raw_measurement;
static int adc_calibrated_voltage;

void app_main(void) {

	adc_oneshot_unit_handle_t adc_handle;

	adc_oneshot_unit_init_cfg_t init_config = {
		.unit_id = ADC_UNIT_1,
		.clk_src = ADC_DIGI_CLK_SRC_DEFAULT,
		.ulp_mode = ADC_ULP_MODE_DISABLE,
	};
	adc_oneshot_new_unit(&init_config, &adc_handle);

	adc_oneshot_chan_cfg_t input_config = {
		.atten = ADC_ATTEN_DB_12,
		.bitwidth = ADC_BITWIDTH_12,
	};
	adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_3, &input_config);

	adc_cali_handle_t cali_handle = NULL;

	adc_cali_curve_fitting_config_t cali_config = {
		.unit_id = ADC_UNIT_1,
		.chan = ADC_CHANNEL_3, // GPIO (3)
		.atten = ADC_ATTEN_DB_12,
		.bitwidth = ADC_BITWIDTH_12,
	};
	adc_cali_create_scheme_curve_fitting(&cali_config, &cali_handle);

	while (true) {
		adc_oneshot_read(adc_handle, ADC_CHANNEL_3, &adc_raw_measurement);
		adc_cali_raw_to_voltage(cali_handle, adc_raw_measurement,
								&adc_calibrated_voltage);
		ESP_LOGI("Main", "Voltage: %d mV (Raw: %d)", adc_calibrated_voltage, adc_raw_measurement);
		sleep(1);
	}
}
