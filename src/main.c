/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <dk_buttons_and_leds.h>

#define STATUS_LED          DK_LED4
#define STATUS_LED_INTERVAL 500

LOG_MODULE_REGISTER(APP);

static void init_leds(void)
{
	int err = dk_leds_init();
	if (err) {
		LOG_ERR("Failed to initialize LEDs module. Error %d", err);
	}
}

void main(void)
{
	int err = 0;
	int cnt = 0;

	LOG_INF("Hello World!");
	LOG_DBG("Board: %s", CONFIG_BOARD);

	init_leds();

	while (true) {
		err = dk_set_led(STATUS_LED, cnt++%2);
		if (err) {
			LOG_ERR("Failed to set the LED. Error %d\n", err);
		}

		k_msleep(STATUS_LED_INTERVAL);
	}
}
