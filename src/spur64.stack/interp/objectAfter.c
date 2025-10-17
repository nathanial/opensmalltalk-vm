/* Extracted from interp.c:41382 (function objectAfter). */

/*	Object parsing.
	1. all objects have at least a word following the header, for a forwarding
	pointer. 2. objects with an overflow size have a preceeing word with a
	saturated slotSize. If the word following
	an object doesn't have a saturated size field it must be a single-header
	object. If the word following
	does have a saturated slotSize it must be the overflow size word. */

	/* SpurMemoryManager>>#objectAfter: */

sqInt
objectAfter(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt followingWord;
    usqInt followingWordAddress;

	if (oopisLessThan(objOop, GIV(oldSpaceStart))) {
		if (oopisGreaterThanOrEqualToandLessThan(objOop, ((GIV(eden)).start), GIV(freeStart))) {
			/* begin objectAfter:limit: */
			followingWordAddress = addressAfter(objOop);
			if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(freeStart))) {
				return GIV(freeStart);
			}
			followingWord = longAt((void *)(followingWordAddress));
			return ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		}
		if (oopisGreaterThanOrEqualToandLessThan(objOop, ((GIV(pastSpace)).start), GIV(pastSpaceStart))) {
			/* begin objectAfter:limit: */
			followingWordAddress = addressAfter(objOop);
			if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(pastSpaceStart))) {
				return GIV(pastSpaceStart);
			}
			followingWord = longAt((void *)(followingWordAddress));
			return ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		}

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(futureSurvivorStart))) {
			return GIV(futureSurvivorStart);
		}
		followingWord = longAt((void *)(followingWordAddress));
		return ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
				? followingWordAddress + BaseHeaderSize
				: followingWordAddress);
	}

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