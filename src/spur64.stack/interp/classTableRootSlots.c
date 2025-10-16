/* Extracted from interp.c:33386 (function classTableRootSlots). */

static sqInt
classTableRootSlots(void)
{
	return 1U << (22 /* classIndexFieldWidth */ - (classTableMajorIndexShift()));
}