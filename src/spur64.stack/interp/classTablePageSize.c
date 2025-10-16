/* Extracted from interp.c:33364 (function classTablePageSize). */

sqInt
classTablePageSize(void)
{
	return 1U << (classTableMajorIndexShift());
}