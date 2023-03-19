/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <dk_buttons_and_leds.h>

#define STATUS_LED          DK_LED4
#define STATUS_LED_INTERVAL 500
#define APP_LED             DK_LED1

LOG_MODULE_REGISTER(APP);

static void led_timer_handler(struct k_timer *timer_id);
K_TIMER_DEFINE(led_timer, led_timer_handler, NULL);

static void led_timer_handler(struct k_timer *timer_id)
{
	int err = 0;
	static int cnt = 0;

	err = dk_set_led(STATUS_LED, cnt++%2);
	if (err) {
		LOG_ERR("Failed to set the LED. Error %d", err);
	}
}

static void button_handler(uint32_t button_state, uint32_t has_changed)
{
	int err;

	uint32_t button_mask = button_state & has_changed;

	if (button_mask & DK_BTN1_MSK)
	{
		LOG_INF("Button 1 has been pushed.");

		err = dk_set_led_off(APP_LED);
		if (err) {
			LOG_ERR("Failed to turn off the LED. Error %d", err);
		}
	}

	if (button_mask & DK_BTN2_MSK)
	{
		LOG_INF("Button 2 has been pushed.");

		err = dk_set_led_on(APP_LED);
		if (err) {
			LOG_ERR("Failed to turn on the LED. Error %d", err);
		}
	}
}

static void init_leds(void)
{
	int err = dk_leds_init();
	if (err) {
		LOG_ERR("Failed to initialize LEDs module. Error %d", err);
	}
}

static void init_buttons(void)
{
	int err = dk_buttons_init(button_handler);
	if (err) {
		LOG_ERR("Failed to initialize buttons module. Error %d", err);
	}
}

void main(void)
{
	LOG_INF("Hello World!");
	LOG_DBG("Board: %s", CONFIG_BOARD);

	init_leds();
	init_buttons();

	// Start status LED timer.
	k_timer_start(&led_timer, K_MSEC(STATUS_LED_INTERVAL), K_MSEC(STATUS_LED_INTERVAL));
}
