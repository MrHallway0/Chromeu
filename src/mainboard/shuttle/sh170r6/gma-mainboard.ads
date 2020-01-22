-- SPDX-License-Identifier: GPL-2.0-only

with HW.GFX.GMA;
with HW.GFX.GMA.Display_Probing;

use HW.GFX.GMA;
use HW.GFX.GMA.Display_Probing;

private package GMA.Mainboard is

	ports : constant Port_List :=
		(HDMI1, -- mainboard HDMI port TBD
		 HDMI2, -- mainboard HDMI port TBD
		 HDMI3, -- mainboard HDMI port TBD
		 DP1,   -- mainboard DP port TBD
		 DP2,   -- mainboard DP port TBD
		 DP3,   -- mainboard DP port TBD
		 others => Disabled);

end GMA.Mainboard;
