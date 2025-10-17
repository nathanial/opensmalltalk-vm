/* Extracted from interp.c:33353 (function classTableMinorIndexMask). */

/*	1024 entries per page (2^10); 22 bit classIndex implies 2^12 pages */
/*	self basicNew classTableMinorIndexMask */

	/* SpurMemoryManager>>#classTableMinorIndexMask */

sqInt
classTableMinorIndexMask(void)
{
	return (1U << (classTableMajorIndexShift())) - 1;
}