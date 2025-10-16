/* Extracted from interp.c:34951 (function expungeFromClassTable). */

static NoDbgRegParms void
expungeFromClassTable(sqInt aBehavior)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classIndex;
    sqInt classTablePage;
    sqInt majorIndex;
    sqInt minorIndex;

	assert(isInClassTable(aBehavior));
	classIndex = (long32At((void *)(aBehavior + 4))) & (identityHashHalfWordMask());
	majorIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());
	minorIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);
	classTablePage = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(majorIndex) << (shiftForWord()))))));
	assert(classTablePage != GIV(classTableFirstPage));
	assert((numSlotsOf(classTablePage)) == (classTablePageSize()));
	assert((fetchPointerofObject(minorIndex, classTablePage)) == aBehavior);

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(classTablePage))
	 && (!(isForwarded(classTablePage))));
	assert(validStorePointerUncheckedArgs(minorIndex, classTablePage, GIV(nilObj)));
	longAtput((void *)((classTablePage + BaseHeaderSize) + ((((usqInt)(minorIndex) << (shiftForWord()))))),GIV(nilObj));

	/* If the removed class is before the classTableIndex, set the
	   classTableIndex to point to the empty slot so as to reuse it asap. */
	if (classIndex < GIV(classTableIndex)) {
		GIV(classTableIndex) = classIndex;
	}

	/* classTableIndex must never index the first page, which is reserved for classes known to the VM. */
	assert(GIV(classTableIndex) >= (1U << (classTableMajorIndexShift())));
}