/* Extracted from interp.c:27602 (function objectAfterMaybeSlimBridgelimit). */

static NoDbgRegParms sqInt
objectAfterMaybeSlimBridgelimit(sqInt objOop, sqInt limit)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt followingWord;
    usqInt followingWordAddress;

	followingWordAddress = addressAfter(objOop);
	if (oopisGreaterThanOrEqualTo(followingWordAddress, limit)) {
		return limit;
	}
	followingWord = longAt((void *)(followingWordAddress));
	return ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
			? ((oopisLessThan(objOop, GIV(oldSpaceStart)))
			 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
					? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
					: followingWordAddress + BaseHeaderSize)
			: followingWordAddress);
}