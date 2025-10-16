/* Extracted from interp.c:40752 (function minSlotsForShortening). */

sqInt
minSlotsForShortening(void)
{
	return 16 /* (allocationUnit * 2) */ / BytesPerOop;
}