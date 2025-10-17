/* Extracted from interp.c:40780 (function nilUnmarkedWeaklingSlots). */

/*	Nil the unmarked slots in the weaklings on the
	weakling stack, finalizing those that lost references.
	Finally, empty the weaklingStack. */
/*	for profiling */

	/* SpurMemoryManager>>#nilUnmarkedWeaklingSlots */

static NeverInline void
nilUnmarkedWeaklingSlots(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt anyUnmarked;
    sqInt i;
    sqInt iSqInt;
    sqInt nextNextPage;
    sqInt nextPage;
    sqInt numOnThisPage;
    usqInt numSlots;
    sqInt numToEnumerate;
    sqInt numToEnumerateOnThisPage;
    sqInt objStackPage;
    sqInt referent;
    sqInt size;
    sqInt toDoLimit;
    sqInt topIndex;
    sqInt weakling;

	eassert(allOldMarkedWeakObjectsOnWeaklingStack());
	if (GIV(weaklingStack) == GIV(nilObj)) {
		return;
	}

	/* begin objStack:from:do: */
	eassert(isValidObjStack(GIV(weaklingStack)));
	size = longAt((void *)((GIV(weaklingStack) + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
	objStackPage = longAt((void *)((GIV(weaklingStack) + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
	while (objStackPage != 0) {
		size += ObjStackLimit;
		assert((fetchPointerofObject(ObjStackTopx, objStackPage)) == ObjStackLimit);
		objStackPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
	}
	numToEnumerate = size;
	objStackPage = GIV(weaklingStack);
	while (numToEnumerate > 0) {
		numOnThisPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
		numToEnumerateOnThisPage = ((numToEnumerate < numOnThisPage) ? numToEnumerate : numOnThisPage);
		topIndex = (numOnThisPage + ObjStackFixedSlots) - 1;
		for (i = topIndex; i >= ((topIndex - numToEnumerateOnThisPage) + 1); i += -1) {
			assert(isWeak(fetchPointerofObject(i, objStackPage)));
			weakling = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));

			/* begin nilUnmarkedWeaklingSlotsIn: */
			anyUnmarked = 0;
			assert(allStrongSlotsOfWeaklingAreMarked(weakling));

			/* N.B. generateToByDoLimitExpression:negative:on: guards against (unsigned)0 - 1 going +ve */
			toDoLimit = ((/* begin numSlotsOf: */
	assert((classIndexOf(weakling)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(weakling + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(weakling - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - 1;
			for (iSqInt = (numStrongSlotsOfWeakling(weakling)); iSqInt <= toDoLimit; iSqInt += 1) {
				referent = longAt((void *)((weakling + BaseHeaderSize) + ((((usqInt)(iSqInt) << (shiftForWord()))))));
				if ((!(referent & (tagMask())))) {
					if (((longAt((void *)(referent))) & (classIndexMask())) == (isForwardedObjectClassIndexPun())) {
						referent = fixFollowedFieldofObjectwithInitialValue(iSqInt, weakling, referent);
					}
					if (!((((referent & (tagMask())) != 0))
						 || (((byteAt((void *)(referent + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0))) {
						/* begin storePointerUnchecked:ofObject:withValue: */
						assert((isNonImmediate(weakling))
						 && (!(isForwarded(weakling))));
						assert(validStorePointerUncheckedArgs(iSqInt, weakling, GIV(nilObj)));
						longAtput((void *)((weakling + BaseHeaderSize) + ((((usqInt)(iSqInt) << (shiftForWord()))))),GIV(nilObj));
						anyUnmarked = 1;
					}
				}
			}
			if (anyUnmarked) {
				assert(GIV(marking));

				/* begin fireFinalization: */
				if (GIV(newFinalization)) {
					queueMourner(weakling);
				}

				/* begin signalFinalization: */
				forceInterruptCheck();
				GIV(pendingFinalizationSignals) += 1;
			}
		}
		numToEnumerate -= numToEnumerateOnThisPage;
		objStackPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
	}

	/* begin emptyObjStack: */
	if (GIV(weaklingStack) == GIV(nilObj)) {
		goto l1;
	}
	eassert(isValidObjStack(GIV(weaklingStack)));

	/* begin storePointer:ofObjStack:withValue: */
	assert((formatOf(GIV(weaklingStack))) == (wordIndexableFormat()));
	longAtput((void *)((GIV(weaklingStack) + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))),0);
	nextPage = longAt((void *)((GIV(weaklingStack) + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
	while (nextPage != 0) {
		nextNextPage = longAt((void *)((nextPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));

		/* begin storePointer:ofObjStack:withValue: */
		assert((formatOf(nextPage)) == (wordIndexableFormat()));
		longAtput((void *)((nextPage + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord()))))),longAt((void *)((GIV(weaklingStack) + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord())))))));

		/* begin storePointer:ofObjStack:withValue: */
		assert((formatOf(nextPage)) == (wordIndexableFormat()));
		longAtput((void *)((nextPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))),0);

		/* begin storePointer:ofObjStack:withValue: */
		assert((formatOf(GIV(weaklingStack))) == (wordIndexableFormat()));
		longAtput((void *)((GIV(weaklingStack) + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord()))))),nextPage);
		nextPage = nextNextPage;
	}

	/* begin storePointer:ofObjStack:withValue: */
	assert((formatOf(GIV(weaklingStack))) == (wordIndexableFormat()));
	longAtput((void *)((GIV(weaklingStack) + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))),0);
	eassert(isValidObjStack(GIV(weaklingStack)));
	/* end emptyObjStack: */
l1:;
}