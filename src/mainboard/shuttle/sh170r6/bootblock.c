/* SPDX-License-Identifier: GPL-2.0-only */

#include <bootblock_common.h>
#include <soc/gpio.h>
#include <superio/ite/common/ite.h>
#include <superio/ite/it8772f/it8772f.h>
#include "gpio.h"

/* IT8772F defs */
#define IT8772F_BASE 0x2e
#define IT8772F_SERIAL_DEV PNP_DEV(IT8772F_BASE, IT8772F_SP1)
#define IT8772F_GPIO_DEV PNP_DEV(IT8772F_BASE, IT8772F_GPIO)
#define IT8772F_SUPERIO_DEV PNP_DEV(IT8772F_BASE, 0)

static void early_config_superio(void)
{
	/* Early SuperIO setup */
	ite_kill_watchdog(IT8772F_GPIO_DEV);
	it8772f_ac_resume_southbridge(IT8772F_SUPERIO_DEV);
	ite_enable_serial(IT8772F_SERIAL_DEV, CONFIG_TTYS0_BASE);
}

static void early_config_gpio(void)
{
	/* This is a hack for FSP because it does things in MemoryInit()
	 * which it shouldn't do. We have to prepare certain gpios here
	 * because of the brokenness in FSP. */
	gpio_configure_pads(early_gpio_table, ARRAY_SIZE(early_gpio_table));
}

void bootblock_mainboard_init(void)
{
	early_config_gpio();
}

void bootblock_mainboard_early_init(void)
{
	early_config_superio();
}
