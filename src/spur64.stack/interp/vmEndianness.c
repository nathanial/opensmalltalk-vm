/* Extracted from interp.c:46892 (function vmEndianness). */

/*	1 = big, 0 = little */

	/* SpurMemoryManager>>#vmEndianness */

sqInt
vmEndianness(void)
{
	return VMBIGENDIAN;
}