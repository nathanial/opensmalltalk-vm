/* Extracted from interp.c:33364 (function classTablePageSize). */

/*	1024 entries per page (2^10); 22 bit classIndex implies 2^12 pages */
/*	self basicNew classTablePageSize */

/* SpurMemoryManager>>#classTablePageSize */

sqInt classTablePageSize(void) { return 1U << (classTableMajorIndexShift()); }