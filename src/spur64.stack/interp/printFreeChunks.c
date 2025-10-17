/* Extracted from interp.c:43080 (function printFreeChunks). */

/*	This version goes through memory, printing all free chunks.
	Other versions go through the free lists. This one will show
	all free chunks even if the free lists are corrupt. */
/*	useful for debugging */

	/* SpurMemoryManager>>#printFreeChunks */

void
printFreeChunks(void)
{
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt numSlots;
    sqInt objOop;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt seenNewFreeChunk;
    usqInt start;

	seenNewFreeChunk = 0;

	/* begin allNewSpaceEntitiesDo: */
	prevPrevObj = (prevObj = null);

	/* After a scavenge eden is empty, futureSpace is empty, and all newSpace objects are
	   in pastSpace.  Objects are allocated in eden.  So enumerate only pastSpace and eden. */
	assert((((pastSpace).start)) < (((eden).start)));
	start = /* startAddressForBridgedHeapEnumeration */
			(pastSpaceStart > (((pastSpace).start))
				? ((pastSpace).start)
				: (freeStart > (((eden).start))
						? ((eden).start)
						: oldSpaceStart));
	if (start > freeStart) {
		goto l2;
	}

	/* begin bridgePastSpaceAndEden */
	if (pastSpaceStart < (((eden).start))) {
		if ((pastSpaceStart + BaseHeaderSize) == (((eden).start))) {
			hackSlimBridgeToat(objectStartingAt(((eden).start)), pastSpaceStart);

			/* And carefully check the assumption */
			assert((objectAfterMaybeSlimBridgelimit(objectInPastSpaceBefore(pastSpaceStart), nilObj)) == (objectStartingAt(((eden).start))));
		}
		else {
			initSegmentBridgeWithBytesat((((eden).start)) - pastSpaceStart, pastSpaceStart);
		}
	}

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(start + (numSlotsFieldByteOffset())));
	objOop = (numSlots == (numSlotsMask())
				? start + BaseHeaderSize
				: start);
	while (oopisLessThan(objOop, freeStart)) {
		if (((longAt((void *)(objOop))) & (classIndexMask())) == (isFreeObjectClassIndexPun())) {
			if (!seenNewFreeChunk) {
				print("NewSpace CONTAINS FREE OBJECT(S)!!");
				cr();
				seenNewFreeChunk = 1;
			}
			printFreeChunk(objOop);
		}
		prevPrevObj = prevObj;
		prevObj = objOop;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, freeStart)) {
			objOop = freeStart;
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(objOop, oldSpaceStart))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l1:;
	}
	/* end allNewSpaceEntitiesDo: */
l2:

	/* begin allOldSpaceEntitiesDo: */
	/* begin allOldSpaceEntitiesFrom:do: */
	assert(isOldObject(nilObj));
	prevPrevObj = (prevObj = null);
	objOop = nilObj;
	while (1) {
		assert((objOop % (allocationUnit())) == 0);
		if (!(oopisLessThan(objOop, endOfMemory))) break;
		assert((long64At((void *)(objOop))) != 0);
		if (((longAt((void *)(objOop))) & (classIndexMask())) == (isFreeObjectClassIndexPun())) {
			printFreeChunk(objOop);
		}
		prevPrevObj = prevObj;
		prevObj = objOop;

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
			objOop = endOfMemory;
			goto l3;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l3:;
	}
}