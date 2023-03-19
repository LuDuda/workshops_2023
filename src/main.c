/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <dk_buttons_and_leds.h>

#include <zephyr/drivers/pwm.h>
#include <zephyr/device.h>

#define STATUS_LED          DK_LED4
#define STATUS_LED_INTERVAL 500
#define APP_LED             DK_LED1

#define LED_THREAD_STACK_SIZE 500
#define LED_THREAD_PRIORITY   5

#define PWM_LEVEL_MAX 10

static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

LOG_MODULE_REGISTER(APP);

static volatile int pwm_level = 0;

static void led_thread(void)
{
	int cnt = 0;

	while (true) {
		int err = dk_set_led(STATUS_LED, cnt++%2);
		if (err) {
			LOG_ERR("Failed to set the LED. Error %d", err);
		}

		k_msleep(STATUS_LED_INTERVAL);
	}
}

static void button_handler(uint32_t button_state, uint32_t has_changed)
{
	int err;

	uint32_t button_mask = button_state & has_changed;

	if (button_mask & DK_BTN1_MSK)
	{
		LOG_INF("Button 1 has been pushed.");

		if (pwm_level > 0) {
			pwm_level--;
		}
	}

	if (button_mask & DK_BTN2_MSK)
	{
		LOG_INF("Button 2 has been pushed.");

		if (pwm_level < PWM_LEVEL_MAX) {
			pwm_level++;
		}
	}

	err = pwm_set_dt(&pwm_led0, 1000, 1000 * pwm_level / PWM_LEVEL_MAX);
	if (err) {
		LOG_ERR("Failed to set pulse width. Error %d", err);
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
}

K_THREAD_DEFINE(led_tid, LED_THREAD_STACK_SIZE,
                (k_thread_entry_t)led_thread, NULL, NULL, NULL,
                LED_THREAD_PRIORITY, 0, 0);
