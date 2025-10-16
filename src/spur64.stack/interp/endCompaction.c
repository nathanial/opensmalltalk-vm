/* Extracted from interp.c:47564 (function endCompaction). */

static void
endCompaction(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt numSlots;
    sqInt objOop;
    sqInt objOopSqInt;
    sqInt prevObj;
    sqInt prevPrevObj;

	/* begin unmarkSurvivingObjectsForCompact */
	/* begin allPastSpaceObjectsDo: */
	/* begin allPastSpaceEntitiesDo: */
	prevPrevObj = (prevObj = null);
	address = ((GIV(pastSpace)).start);

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
	objOopSqInt = (numSlots == (numSlotsMask())
				? address + BaseHeaderSize
				: address);
	while (oopisLessThan(objOopSqInt, GIV(pastSpaceStart))) {
		assert(isEnumerableObjectNoAssert(objOopSqInt));
		if ((byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) {
			/* begin setIsMarkedOf:to: */
			assert(!(isFreeObject(objOopSqInt)));
			byteAtput((void *)(objOopSqInt + (markBitsByteOffset())),(byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
		}
		prevPrevObj = prevObj;
		prevObj = objOopSqInt;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(objOopSqInt);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(pastSpaceStart))) {
			objOopSqInt = GIV(pastSpaceStart);
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

	/* begin endSlidingCompaction */
	GIV(gcPhaseInProgress) = 0;
	if (GIV(rememberedSetSize) > 0) {
		objOop = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(RememberedSetRootIndex) << (shiftForWord()))))));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(objOop))
		 && (!(isForwarded(objOop))));
		assert(validStorePointerUncheckedArgs(0, objOop, GIV(firstFieldOfRememberedSet)));
		longAtput((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))),GIV(firstFieldOfRememberedSet));
	}
	setIsPinnedOfto(longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(RememberedSetRootIndex) << (shiftForWord())))))), 1);

	/* begin relocateRememberedSet */
	GIV(rememberedSet) = firstIndexableField(longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(RememberedSetRootIndex) << (shiftForWord())))))));
	if (/* savedFirstFieldsSpaceWasAllocated */
		GIV(savedFirstFieldsSpaceNotInOldSpace)
	 && (oopisGreaterThan((GIV(savedFirstFieldsSpace).start), GIV(nilObj)))) {
		sqDeallocateMemorySegmentAtOfSize(((void *)((GIV(savedFirstFieldsSpace).start))), ((GIV(savedFirstFieldsSpace).limit)) - ((GIV(savedFirstFieldsSpace).start)));
	}
}