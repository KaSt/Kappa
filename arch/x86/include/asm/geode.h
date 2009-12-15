/*
 * AMD Geode definitions
 * Copyright (C) 2006, Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 */

#ifndef _ASM_X86_GEODE_H
#define _ASM_X86_GEODE_H

#include <asm/processor.h>
#include <linux/io.h>
#include <linux/cs5535.h>

/* Generic southbridge functions */

#define GEODE_DEV_PMS 0
#define GEODE_DEV_ACPI 1
#define GEODE_DEV_GPIO 2
#define GEODE_DEV_MFGPT 3

extern int geode_get_dev_base(unsigned int dev);

/* Useful macros */
#define geode_pms_base()	geode_get_dev_base(GEODE_DEV_PMS)
#define geode_acpi_base()	geode_get_dev_base(GEODE_DEV_ACPI)
#define geode_gpio_base()	geode_get_dev_base(GEODE_DEV_GPIO)
#define geode_mfgpt_base()	geode_get_dev_base(GEODE_DEV_MFGPT)

/* MSRS */

#define MSR_GLIU_P2D_RO0	0x10000029

#define MSR_LX_GLD_MSR_CONFIG	0x48002001
#define MSR_LX_MSR_PADSEL	0x48002011	/* NOT 0x48000011; the data
						 * sheet has the wrong value */
#define MSR_GLCP_SYS_RSTPLL	0x4C000014
#define MSR_GLCP_DOTPLL		0x4C000015

#define MSR_LBAR_SMB		0x5140000B
#define MSR_LBAR_GPIO		0x5140000C
#define MSR_LBAR_MFGPT		0x5140000D
#define MSR_LBAR_ACPI		0x5140000E
#define MSR_LBAR_PMS		0x5140000F

#define MSR_LX_SPARE_MSR	0x80000011	/* DC-specific */

#define MSR_GX_GLD_MSR_CONFIG	0xC0002001
#define MSR_GX_MSR_PADSEL	0xC0002011

/* Resource Sizes */

#define LBAR_GPIO_SIZE		0xFF
#define LBAR_MFGPT_SIZE		0x40
#define LBAR_ACPI_SIZE		0x40
#define LBAR_PMS_SIZE		0x80

/* ACPI registers (PMS block) */

/*
 * PM1_EN is only valid when VSA is enabled for 16 bit reads.
 * When VSA is not enabled, *always* read both PM1_STS and PM1_EN
 * with a 32 bit read at offset 0x0
 */

#define PM1_STS			0x00
#define PM1_EN			0x02
#define PM1_CNT			0x08
#define PM2_CNT			0x0C
#define PM_TMR			0x10
#define PM_GPE0_STS		0x18
#define PM_GPE0_EN		0x1C

/* PMC registers (PMS block) */

#define PM_SSD			0x00
#define PM_SCXA			0x04
#define PM_SCYA			0x08
#define PM_OUT_SLPCTL		0x0C
#define PM_SCLK			0x10
#define PM_SED			0x1
#define PM_SCXD			0x18
#define PM_SCYD			0x1C
#define PM_IN_SLPCTL		0x20
#define PM_WKD			0x30
#define PM_WKXD			0x34
#define PM_RD			0x38
#define PM_WKXA			0x3C
#define PM_FSD			0x40
#define PM_TSD			0x44
#define PM_PSD			0x48
#define PM_NWKD			0x4C
#define PM_AWKD			0x50
#define PM_SSC			0x54

static inline u32 geode_gpio(unsigned int nr)
{
	BUG_ON(nr > 28);
	return 1 << nr;
}

extern void geode_gpio_set(u32, unsigned int);
extern void geode_gpio_clear(u32, unsigned int);
extern int geode_gpio_isset(u32, unsigned int);
extern void geode_gpio_setup_event(unsigned int, int, int);
extern void geode_gpio_set_irq(unsigned int, unsigned int);

static inline void geode_gpio_event_irq(unsigned int gpio, int pair)
{
	geode_gpio_setup_event(gpio, pair, 0);
}

static inline void geode_gpio_event_pme(unsigned int gpio, int pair)
{
	geode_gpio_setup_event(gpio, pair, 1);
}

/* Specific geode tests */

static inline int is_geode_gx(void)
{
	return ((boot_cpu_data.x86_vendor == X86_VENDOR_NSC) &&
		(boot_cpu_data.x86 == 5) &&
		(boot_cpu_data.x86_model == 5));
}

static inline int is_geode_lx(void)
{
	return ((boot_cpu_data.x86_vendor == X86_VENDOR_AMD) &&
		(boot_cpu_data.x86 == 5) &&
		(boot_cpu_data.x86_model == 10));
}

static inline int is_geode(void)
{
	return (is_geode_gx() || is_geode_lx());
}

static inline void geode_mfgpt_write(int timer, u16 reg, u16 value)
{
	u32 base = geode_get_dev_base(GEODE_DEV_MFGPT);
	outw(value, base + reg + (timer * 8));
}

static inline u16 geode_mfgpt_read(int timer, u16 reg)
{
	u32 base = geode_get_dev_base(GEODE_DEV_MFGPT);
	return inw(base + reg + (timer * 8));
}

extern int geode_mfgpt_toggle_event(int timer, int cmp, int event, int enable);
extern int geode_mfgpt_set_irq(int timer, int cmp, int *irq, int enable);
extern int geode_mfgpt_alloc_timer(int timer, int domain);

#define geode_mfgpt_setup_irq(t, c, i) geode_mfgpt_set_irq((t), (c), (i), 1)
#define geode_mfgpt_release_irq(t, c, i) geode_mfgpt_set_irq((t), (c), (i), 0)

#ifdef CONFIG_GEODE_MFGPT_TIMER
extern int __init mfgpt_timer_setup(void);
#else
static inline int mfgpt_timer_setup(void) { return 0; }
#endif

#endif /* _ASM_X86_GEODE_H */
