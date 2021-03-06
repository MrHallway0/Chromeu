/* SPDX-License-Identifier: GPL-2.0-only */

/* IPU3 input system - Device 05, Function 0 */
Device (IMGU)
{
	Name (_ADR, 0x00050000)
	Name (_DDN, "Imaging Unit")
	Name (_CCA, ZERO)
	Name (CAMD, 0x01)
	Method (_STA, 0, NotSerialized)  /* _STA: Status */
		{
			Return (0x0B)
		}
}
