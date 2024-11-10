/*
 * Copyright (c) 2014 Justin Simon
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <stdio.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util.h>

#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/i2c/target/eeprom.h>
#include <zephyr/drivers/gpio.h>

#include "zephyr/app_version.h"

LOG_MODULE_REGISTER(main, CONFIG_MICROEXERCISER_LOG_LEVEL);

#define NODE_EP0 DT_NODELABEL(eeprom0)
#define NODE_EP1 DT_NODELABEL(eeprom1)

#define TEST_DATA_SIZE	20
static const uint8_t eeprom_0_data[TEST_DATA_SIZE] = "0123456789abcdefghij";
static const uint8_t eeprom_1_data[TEST_DATA_SIZE] = "jihgfedcba9876543210";


/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
    int ret;
    bool led_state = true;
    const struct device *const eeprom_0 = DEVICE_DT_GET(NODE_EP0);
    const struct device *const i2c_0 = DEVICE_DT_GET(DT_BUS(NODE_EP0));
    int addr_0 = DT_REG_ADDR(NODE_EP0);
    uint8_t addr_0_width = DT_PROP_OR(NODE_EP0, address_width, 8);
	const struct device *const eeprom_1 = DEVICE_DT_GET(NODE_EP1);
	const struct device *const i2c_1 = DEVICE_DT_GET(DT_BUS(NODE_EP1));
	int addr_1 = DT_REG_ADDR(NODE_EP1);
	uint8_t addr_1_width = DT_PROP_OR(NODE_EP1, address_width, 8);

#ifdef CONFIG_MICROEXERCISER_BOOT_BANNER
    printk("*** uExerciser firmware version " APP_VERSION_STRING " ***\n");
#endif /* CONFIG_MICROEXERCISER_BOOT_BANNER */

    LOG_DBG("Found EEPROM 0 on I2C bus device %s at addr %02x\n",
             i2c_0->name, addr_0);

    LOG_DBG("Found EEPROM 1 on I2C bus device %s at addr %02x\n",
		 i2c_1->name, addr_1);

    eeprom_target_program(eeprom_0, eeprom_0_data, TEST_DATA_SIZE);
    i2c_target_driver_register(eeprom_0);

    eeprom_target_program(eeprom_1, eeprom_1_data, TEST_DATA_SIZE);
    i2c_target_driver_register(eeprom_1);

    if (!gpio_is_ready_dt(&led)) {
        return 0;
    }

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        return 0;
    }

    LOG_DBG("DBG: system is up and running...\n");

    while (1) {
        ret = gpio_pin_toggle_dt(&led);
        if (ret < 0) {
            return 0;
        }

        led_state = !led_state;
        LOG_DBG("LED state: %s\n", led_state ? "ON" : "OFF");
        k_msleep(SLEEP_TIME_MS);
    }

    /* Detach EEPROM */
    ret = i2c_target_driver_unregister(eeprom_0);
//    zassert_equal(ret, 0, "Failed to unregister EEPROM 0");

    return 0;
}
