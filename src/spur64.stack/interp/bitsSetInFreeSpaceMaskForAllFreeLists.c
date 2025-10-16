/* Extracted from interp.c:31760 (function bitsSetInFreeSpaceMaskForAllFreeLists). */

static sqInt
bitsSetInFreeSpaceMaskForAllFreeLists(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;

	for (i = 0; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
		if (((GIV(freeLists)[i]) != 0)
		 && ((!((1ULL << i) & GIV(freeListsMask))))) {
			return 0;
		}
	}
	return 1;
}