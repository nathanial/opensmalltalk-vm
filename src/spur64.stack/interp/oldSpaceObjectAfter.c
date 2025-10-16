/* Extracted from interp.c:42086 (function oldSpaceObjectAfter). */

sqInt
oldSpaceObjectAfter(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt followingWord;
    usqInt followingWordAddress;

	/* begin objectAfter:limit: */
	followingWordAddress = addressAfter(objOop);
	if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
		return GIV(endOfMemory);
	}
	followingWord = longAt((void *)(followingWordAddress));
	return ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
			? followingWordAddress + BaseHeaderSize
			: followingWordAddress);
}