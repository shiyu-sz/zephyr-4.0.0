/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define SHELL_TASK_STACK_SIZE (1024)
K_THREAD_STACK_DEFINE(shell_thread_stack, SHELL_TASK_STACK_SIZE);
static struct k_thread shell_thread_data;

#define LED_TASK_STACK_SIZE (1024)
K_THREAD_STACK_DEFINE(led_thread_stack, LED_TASK_STACK_SIZE);
static struct k_thread led_thread_data;

static int cmd1_handler(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(sh);
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "cmd1 executed");

	return 0;
}

SHELL_SUBCMD_SET_CREATE(sub_app_cmd, (app_cmd));

/* Create a set of subcommands for "section_cmd cm1". */
SHELL_SUBCMD_SET_CREATE(sub_app_cmd1, (app_cmd, cmd1));

/* Add command to the set. Subcommand set is identify by parent shell command. */
SHELL_SUBCMD_ADD((app_cmd), cmd1, &sub_app_cmd1, "help for cmd1", cmd1_handler, 1, 0);

SHELL_CMD_REGISTER(app_cmd, &sub_app_cmd,
		   "Demo command using section for subcommand registration", NULL);

void shell_task(void *arg1, void *arg2, void *arg3)
{
#if DT_NODE_HAS_COMPAT(DT_CHOSEN(zephyr_shell_uart), zephyr_cdc_acm_uart)
	const struct device *dev;
	uint32_t dtr = 0;

	dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));
	if (!device_is_ready(dev) || usb_enable(NULL)) {
		return 0;
	}

	while (!dtr) {
		uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
		k_sleep(K_MSEC(100));
	}
#endif
}

void led_task(void *arg1, void *arg2, void *arg3)
{
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return 0;
		}

		led_state = !led_state;
		// printf("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(SLEEP_TIME_MS);
	}
}

int main(void)
{

	k_thread_create(&shell_thread_data, shell_thread_stack, SHELL_TASK_STACK_SIZE,
		shell_task, NULL, NULL, NULL, K_PRIO_COOP(7), 0, K_NO_WAIT);

	k_thread_create(&led_thread_data, led_thread_stack, LED_TASK_STACK_SIZE,
		led_task, NULL, NULL, NULL, K_PRIO_COOP(7), 0, K_NO_WAIT);

	while (1) {
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
