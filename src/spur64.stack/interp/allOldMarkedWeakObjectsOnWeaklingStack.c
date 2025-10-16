/* Extracted from interp.c:30665 (function allOldMarkedWeakObjectsOnWeaklingStack). */

static sqInt
allOldMarkedWeakObjectsOnWeaklingStack(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt objOop;
    sqInt prevObj;
    sqInt prevPrevObj;

	/* begin allOldSpaceEntitiesDo: */
	/* begin allOldSpaceEntitiesFrom:do: */
	assert(isOldObject(GIV(nilObj)));
	prevPrevObj = (prevObj = null);
	objOop = GIV(nilObj);
	while (1) {
		assert((objOop % (allocationUnit())) == 0);
		if (!(oopisLessThan(objOop, GIV(endOfMemory)))) break;
		assert((long64At((void *)(objOop))) != 0);
		if ((isWeakNonImm(objOop))
		 && (((byteAt((void *)(objOop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0)) {
			if (!(isonObjStack(objOop, GIV(weaklingStack)))) {
				return 0;
			}
		}
		prevPrevObj = prevObj;
		prevObj = objOop;

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
			objOop = GIV(endOfMemory);
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l1:;
	}
	return 1;
}