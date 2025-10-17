/* Extracted from interp.c:31760 (function bitsSetInFreeSpaceMaskForAllFreeLists). */

	/* SpurMemoryManager>>#bitsSetInFreeSpaceMaskForAllFreeLists */

static sqInt
bitsSetInFreeSpaceMaskForAllFreeLists(void)
{
    sqInt i;

	for (i = 0; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
		if (((freeLists[i]) != 0)
		 && ((!((1ULL << i) & freeListsMask)))) {
			return 0;
		}
	}
	return 1;
}