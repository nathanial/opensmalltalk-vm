/* Extracted from interp.c:42955 (function printForwarders). */

/*	useful for VM debugging */

	/* SpurMemoryManager>>#printForwarders */

void
printForwarders(void)
{
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt numSlots;
    sqInt objOopSqInt;
    sqInt prevObj;
    sqInt prevPrevObj;
    usqInt start;

	/* begin allHeapEntitiesDo: */
	/* begin allOldSpaceEntitiesDo: */
	/* begin allOldSpaceEntitiesFrom:do: */
	assert(isOldObject(nilObj));
	prevPrevObj = (prevObj = null);
	objOopSqInt = nilObj;
	while (1) {
		assert((objOopSqInt % (allocationUnit())) == 0);
		if (!(oopisLessThan(objOopSqInt, endOfMemory))) break;
		assert((long64At((void *)(objOopSqInt))) != 0);
		if (((longAt((void *)(objOopSqInt))) & (classIndexMask())) == (isForwardedObjectClassIndexPun())) {
			printHex(objOopSqInt);
			cr();
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
		goto l3;
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
	objOopSqInt = (numSlots == (numSlotsMask())
				? start + BaseHeaderSize
				: start);
	while (oopisLessThan(objOopSqInt, freeStart)) {
		if (((longAt((void *)(objOopSqInt))) & (classIndexMask())) == (isForwardedObjectClassIndexPun())) {
			printHex(objOopSqInt);
			cr();
		}
		prevPrevObj = prevObj;
		prevObj = objOopSqInt;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(objOopSqInt);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, freeStart)) {
			objOopSqInt = freeStart;
			goto l2;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(objOopSqInt, oldSpaceStart))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l2:;
	}
	/* end allNewSpaceEntitiesDo: */
l3:;
}