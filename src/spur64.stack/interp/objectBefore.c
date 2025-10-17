/* Extracted from interp.c:41438 (function objectBefore). */

/*	useful for debugging */

	/* SpurMemoryManager>>#objectBefore: */

sqInt
objectBefore(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt numSlots;
    sqInt objOopSqInt;
    sqInt prev;
    sqInt prevObj;
    sqInt prevPrevObj;
    usqInt start;

	prev = null;
	if (oopisLessThan(objOop, GIV(oldSpaceStart))) {
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
			if (oopisGreaterThanOrEqualTo(objOopSqInt, objOop)) {
				return prev;
			}
			prev = objOopSqInt;
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
		return prev;
	}

	/* begin allOldSpaceEntitiesDo: */
	/* begin allOldSpaceEntitiesFrom:do: */
	assert(isOldObject(GIV(nilObj)));
	prevPrevObj = (prevObj = null);
	objOopSqInt = GIV(nilObj);
	while (1) {
		assert((objOopSqInt % (allocationUnit())) == 0);
		if (!(oopisLessThan(objOopSqInt, GIV(endOfMemory)))) break;
		assert((long64At((void *)(objOopSqInt))) != 0);
		if (oopisGreaterThanOrEqualTo(objOopSqInt, objOop)) {
			return prev;
		}
		prev = objOopSqInt;
		prevPrevObj = prevObj;
		prevObj = objOopSqInt;

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOopSqInt);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
			objOopSqInt = GIV(endOfMemory);
			goto l3;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l3:;
	}
	return prev;
}