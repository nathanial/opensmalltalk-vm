/* Extracted from interp.c:33353 (function classTableMinorIndexMask). */

sqInt
classTableMinorIndexMask(void)
{
	return (1U << (classTableMajorIndexShift())) - 1;
}