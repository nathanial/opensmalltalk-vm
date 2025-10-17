/* Extracted from interp.c:27511 (function maxSlotsForAlloc). */

/*	Answer the maximum number of slots we are willing to attempt to allocate
	in an object.
	Must fit in 56-bits; c.f. bytesInObject: */

	/* Spur64BitMemoryManager>>#maxSlotsForAlloc */

static sqInt
maxSlotsForAlloc(void)
{
	return 0x10000000000LL;
}