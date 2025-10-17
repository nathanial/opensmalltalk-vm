/* Extracted from interp.c:34869 (function existInstancesInNewSpaceOf). */

	/* SpurMemoryManager>>#existInstancesInNewSpaceOf: */

static NoDbgRegParms sqInt
existInstancesInNewSpaceOf(sqInt classObj)
{
    sqInt classIndex;
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt numSlots;
    sqInt objOopSqInt;
    sqInt prevObj;
    sqInt prevPrevObj;
    usqInt start;

	classIndex = (long32At((void *)(classObj + 4))) & (identityHashHalfWordMask());

	/* begin allNewSpaceObjectsDo: */
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
	objOopSqInt = (numSlots == (numSlotsMask())
				? start + BaseHeaderSize
				: start);
	while (oopisLessThan(objOopSqInt, freeStart)) {
		assert(isBridgeOrEnumerableObjectNoAssert(objOopSqInt));
		if (((longAt((void *)(objOopSqInt))) & (classIndexMask())) == classIndex) {
			return 1;
		}
		prevPrevObj = prevObj;
		prevObj = objOopSqInt;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(objOopSqInt);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, freeStart)) {
			objOopSqInt = freeStart;
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(objOopSqInt, oldSpaceStart))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l1:;
	}
	/* end allNewSpaceEntitiesDo: */
l2:
	return 0;
}