/* Extracted from interp.c:62030 (function removeFirstLinkOfList). */

/*	Remove the first process from the given linked list. */

	/* StackInterpreter>>#removeFirstLinkOfList: */

static NoDbgRegParms sqInt
removeFirstLinkOfList(sqInt aList)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt first;
    sqInt last;
    sqInt next;

	assert(!((isForwarded(aList))));
	first = longAt((void *)((aList + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))));
	last = longAt((void *)((aList + BaseHeaderSize) + ((((usqInt)(LastLinkIndex) << (shiftForWord()))))));
	assert(!((isForwarded(first))));
	assert(!((isForwarded(last))));
	if (first == last) {
		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(aList))
		 && (!(isForwarded(aList))));
		assert(validStorePointerUncheckedArgs(FirstLinkIndex, aList, nilObj));
		longAtput((void *)((aList + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))),nilObj);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(aList))
		 && (!(isForwarded(aList))));
		assert(validStorePointerUncheckedArgs(LastLinkIndex, aList, nilObj));
		longAtput((void *)((aList + BaseHeaderSize) + ((((usqInt)(LastLinkIndex) << (shiftForWord()))))),nilObj);
	}
	else {
		next = longAt((void *)((first + BaseHeaderSize) + ((((usqInt)(NextLinkIndex) << (shiftForWord()))))));

		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(FirstLinkIndex, aList, next));
		assert(isNonImmediate(aList));
		if (oopisGreaterThanOrEqualTo(aList, oldSpaceStart)) {
			if (/* isYoung: */
				((!(next & (tagMask()))))
			 && (oopisLessThan(next, oldSpaceStart))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(aList + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(aList);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((aList + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))),next);
	}

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(first))
	 && (!(isForwarded(first))));
	assert(validStorePointerUncheckedArgs(NextLinkIndex, first, nilObj));
	longAtput((void *)((first + BaseHeaderSize) + ((((usqInt)(NextLinkIndex) << (shiftForWord()))))),nilObj);
	return first;
}