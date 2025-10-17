/* Extracted from interp.c:33451 (function clearLeakMapAndMapAccessibleFreeSpace). */

/*	Perform an integrity/leak check using the heapMap. Set a bit at each free
	chunk's header. */

	/* SpurMemoryManager>>#clearLeakMapAndMapAccessibleFreeSpace */

static void
clearLeakMapAndMapAccessibleFreeSpace(void)
{
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt objOopSqInt;
    sqInt prevObj;
    sqInt prevPrevObj;

	clearHeapMap();

	/* begin allOldSpaceEntitiesFrom:do: */
	assert(isOldObject(nilObj));
	prevPrevObj = (prevObj = null);
	objOopSqInt = nilObj;
	while (1) {
		assert((objOopSqInt % (allocationUnit())) == 0);
		if (!(oopisLessThan(objOopSqInt, endOfMemory))) break;
		assert((long64At((void *)(objOopSqInt))) != 0);
		if (((longAt((void *)(objOopSqInt))) & (classIndexMask())) == (isFreeObjectClassIndexPun())) {
			heapMapAtWordPut(pointerForOop(objOopSqInt), 1);
		}
		prevPrevObj = prevObj;
		prevObj = objOopSqInt;

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOopSqInt);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
			objOopSqInt = endOfMemory;
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l1:;
	}
}