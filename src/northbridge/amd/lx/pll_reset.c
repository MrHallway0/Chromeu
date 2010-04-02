/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

static void pll_reset(char manualconf)
{
	msr_t msrGlcpSysRstpll;

	msrGlcpSysRstpll = rdmsr(GLCP_SYS_RSTPLL);

	print_debug("_MSR GLCP_SYS_RSTPLL (");
	print_debug_hex32(GLCP_SYS_RSTPLL);
	print_debug(") value is: ");
	print_debug_hex32(msrGlcpSysRstpll.hi);
	print_debug(":");
	print_debug_hex32(msrGlcpSysRstpll.lo);
	print_debug("\n");
	post_code(POST_PLL_INIT);

	if (!(msrGlcpSysRstpll.lo & (1 << RSTPLL_LOWER_SWFLAGS_SHIFT))) {
		print_debug("Configuring PLL\n");
		if (manualconf) {
			post_code(POST_PLL_MANUAL);
			/* CPU and GLIU mult/div (GLMC_CLK = GLIU_CLK / 2)  */
			msrGlcpSysRstpll.hi = PLLMSRhi;

			/* Hold Count - how long we will sit in reset */
			msrGlcpSysRstpll.lo = PLLMSRlo;
		} else {
			/*automatic configuration (straps) */
			post_code(POST_PLL_STRAP);
			msrGlcpSysRstpll.lo &=
			    ~(0xFF << RSTPPL_LOWER_HOLD_COUNT_SHIFT);
			msrGlcpSysRstpll.lo |=
			    (0xDE << RSTPPL_LOWER_HOLD_COUNT_SHIFT);
			msrGlcpSysRstpll.lo &=
			    ~(RSTPPL_LOWER_COREBYPASS_SET |
			      RSTPPL_LOWER_MBBYPASS_SET);
			msrGlcpSysRstpll.lo |=
			    RSTPPL_LOWER_COREPD_SET | RSTPPL_LOWER_CLPD_SET;
		}
		/* Use SWFLAGS to remember: "we've already been here"  */
		msrGlcpSysRstpll.lo |= (1 << RSTPLL_LOWER_SWFLAGS_SHIFT);

		/* "reset the chip" value */
		msrGlcpSysRstpll.lo |= RSTPPL_LOWER_CHIP_RESET_SET;
		wrmsr(GLCP_SYS_RSTPLL, msrGlcpSysRstpll);

		/*      You should never get here..... The chip has reset. */
		print_debug("CONFIGURING PLL FAILURE\n");
		post_code(POST_PLL_RESET_FAIL);
		__asm__ __volatile__("hlt\n");

	}
	print_debug("Done pll_reset\n");
	return;
}

#if 0 // Unused
static unsigned int CPUSpeed(void)
{
	unsigned int speed;
	msr_t msr;

	msr = rdmsr(GLCP_SYS_RSTPLL);
	speed = ((((msr.hi >> RSTPLL_UPPER_CPUMULT_SHIFT) & 0x1F) + 1) * 333) / 10;
	if ((((((msr.hi >> RSTPLL_UPPER_CPUMULT_SHIFT) & 0x1F) + 1) * 333) % 10) > 5) {
		++speed;
	}
	return (speed);
}
#endif

static unsigned int GeodeLinkSpeed(void)
{
	unsigned int speed;
	msr_t msr;

	msr = rdmsr(GLCP_SYS_RSTPLL);
	speed = ((((msr.hi >> RSTPLL_UPPER_GLMULT_SHIFT) & 0x1F) + 1) * 333) / 10;
	if ((((((msr.hi >> RSTPLL_UPPER_GLMULT_SHIFT) & 0x1F) + 1) * 333) % 10) > 5) {
		++speed;
	}
	return (speed);
}

#if 0 // Unused
static unsigned int PCISpeed(void)
{
	msr_t msr;

	msr = rdmsr(GLCP_SYS_RSTPLL);
	if (msr.hi & (1 << RSTPPL_LOWER_PCISPEED_SHIFT)) {
		return (66);
	} else {
		return (33);
	}
}
#endif
