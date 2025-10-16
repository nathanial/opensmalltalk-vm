/* Extracted from interp.c:34869 (function existInstancesInNewSpaceOf). */

static NoDbgRegParms sqInt
existInstancesInNewSpaceOf(sqInt classObj)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
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
	assert((((GIV(pastSpace)).start)) < (((GIV(eden)).start)));
	start = /* startAddressForBridgedHeapEnumeration */
			(GIV(pastSpaceStart) > (((GIV(pastSpace)).start))
				? ((GIV(pastSpace)).start)
				: (GIV(freeStart) > (((GIV(eden)).start))
						? ((GIV(eden)).start)
						: GIV(oldSpaceStart)));
	if (start > GIV(freeStart)) {
		goto l2;
	}

	/* begin bridgePastSpaceAndEden */
	if (GIV(pastSpaceStart) < (((GIV(eden)).start))) {
		if ((GIV(pastSpaceStart) + BaseHeaderSize) == (((GIV(eden)).start))) {
			hackSlimBridgeToat(objectStartingAt(((GIV(eden)).start)), GIV(pastSpaceStart));

			/* And carefully check the assumption */
			assert((objectAfterMaybeSlimBridgelimit(objectInPastSpaceBefore(GIV(pastSpaceStart)), GIV(nilObj))) == (objectStartingAt(((GIV(eden)).start))));
		}
		else {
			initSegmentBridgeWithBytesat((((GIV(eden)).start)) - GIV(pastSpaceStart), GIV(pastSpaceStart));
		}
	}

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(start + (numSlotsFieldByteOffset())));
	objOopSqInt = (numSlots == (numSlotsMask())
				? start + BaseHeaderSize
				: start);
	while (oopisLessThan(objOopSqInt, GIV(freeStart))) {
		assert(isBridgeOrEnumerableObjectNoAssert(objOopSqInt));
		if (((longAt((void *)(objOopSqInt))) & (classIndexMask())) == classIndex) {
			return 1;
		}
		prevPrevObj = prevObj;
		prevObj = objOopSqInt;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(objOopSqInt);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(freeStart))) {
			objOopSqInt = GIV(freeStart);
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(objOopSqInt, GIV(oldSpaceStart)))
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