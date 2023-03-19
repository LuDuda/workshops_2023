/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(APP);

void main(void)
{
	LOG_INF("Hello World!");
	LOG_DBG("Board: %s", CONFIG_BOARD);
}
