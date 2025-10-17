/* Extracted from interp.c:34771 (function enterIntoClassTable). */

/*	Enter aBehavior into the class table and answer 0. Otherwise answer a
	primitive failure code. */

	/* SpurMemoryManager>>#enterIntoClassTable: */

static NoDbgRegParms sqInt
enterIntoClassTable(sqInt aBehavior)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt initialMajorIndex;
    sqInt majorIndex;
    sqInt minorIndex;
    usqInt p;
    sqInt page;
    sqInt toDoLimit;
    usqInt toDoLimitUsqInt;

	majorIndex = ((usqInt)(classTableIndex)) >> (classTableMajorIndexShift());
	initialMajorIndex = majorIndex;

	/* classTableIndex should never index the first page; it's reserved for known classes */
	assert(initialMajorIndex > 0);
	minorIndex = classTableIndex & ((1U << (classTableMajorIndexShift())) - 1);
	while (1) {
		page = longAt((void *)((hiddenRootsObj + BaseHeaderSize) + ((((usqInt)(majorIndex) << (shiftForWord()))))));
		if (page == nilObj) {
			/* begin allocateSlotsInOldSpace:format:classIndex: */
			page = allocateSlotsInOldSpacebytesformatclassIndex(1U << (classTableMajorIndexShift()), /* objectBytesForSlots: */
					(1U << (classTableMajorIndexShift())
						? ((((usqInt)((1U << (classTableMajorIndexShift()))) << (shiftForWord())))) + (((1U << (classTableMajorIndexShift())) >= (numSlotsMask())
		? BaseHeaderSize + BaseHeaderSize
		: BaseHeaderSize))
						: 8 /* allocationUnit */ + BaseHeaderSize), arrayFormat(), arrayClassIndexPun());
			if (!page) {
				return PrimErrNoMemory;
			}

			/* begin fillObj:numSlots:with: */
			assert(oopisLessThan(((page + BaseHeaderSize) + ((1U << (classTableMajorIndexShift())) * BytesPerOop)) - 1, addressAfter(page)));
			toDoLimitUsqInt = ((usqInt)(((page + BaseHeaderSize) + ((1U << (classTableMajorIndexShift())) * BytesPerOop)) - 1));
			for (p = (((usqInt)(page + BaseHeaderSize))); p <= toDoLimitUsqInt; p += 8 /* allocationUnit */) {
				longAtput((void *)(p),nilObj);
			}

			/* begin storePointer:ofObject:withValue: */
			assert(validStorePointerArgs(majorIndex, hiddenRootsObj, page));
			assert(isNonImmediate(hiddenRootsObj));
			if (oopisGreaterThanOrEqualTo(hiddenRootsObj, oldSpaceStart)) {
				if (/* isYoung: */
					((!(page & (tagMask()))))
				 && (oopisLessThan(page, oldSpaceStart))) {
					/* begin possibleRootStoreInto: */
					if (!((byteAt((void *)(hiddenRootsObj + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
						remember(hiddenRootsObj);
					}
				}
			}

			/* most stores into young objects */
			longAtput((void *)((hiddenRootsObj + BaseHeaderSize) + ((((usqInt)(majorIndex) << (shiftForWord()))))),page);
			numClassTablePages += 1;
			minorIndex = 0;
		}
		toDoLimit = (1U << (classTableMajorIndexShift())) - 1;
		for (i = minorIndex; i <= toDoLimit; i += 1) {
			if ((longAt((void *)((page + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))))) == nilObj) {
				classTableIndex = ((((usqInt)(majorIndex) << (classTableMajorIndexShift())))) + i;

				/* classTableIndex must never index the first page, which is reserved for classes known to the VM. */
				assert(classTableIndex >= (1U << (classTableMajorIndexShift())));

				/* begin storePointer:ofObject:withValue: */
				assert(validStorePointerArgs(i, page, aBehavior));
				assert(isNonImmediate(page));
				if (oopisGreaterThanOrEqualTo(page, oldSpaceStart)) {
					if (/* isYoung: */
						((!(aBehavior & (tagMask()))))
					 && (oopisLessThan(aBehavior, oldSpaceStart))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(page + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(page);
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((page + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),aBehavior);

				/* begin setHashBitsOf:to: */
				long32Atput((void *)(aBehavior + 4),((((long32At((void *)(aBehavior + 4))) | (identityHashHalfWordMask())) - (identityHashHalfWordMask()))) + (classTableIndex & (identityHashHalfWordMask())));
				assert((classAtIndex(rawHashBitsOf(aBehavior))) == aBehavior);
				return 0;
			}
		}
		majorIndex = ((((majorIndex + 1) & (classIndexMask())) < 1) ? 1 : ((majorIndex + 1) & (classIndexMask())));
		if (majorIndex == initialMajorIndex) {
			return PrimErrLimitExceeded;
		}
	}
	return 0;
}