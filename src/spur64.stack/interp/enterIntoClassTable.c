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

	majorIndex = ((usqInt)(GIV(classTableIndex))) >> (classTableMajorIndexShift());
	initialMajorIndex = majorIndex;

	/* classTableIndex should never index the first page; it's reserved for known classes */
	assert(initialMajorIndex > 0);
	minorIndex = GIV(classTableIndex) & ((1U << (classTableMajorIndexShift())) - 1);
	while (1) {
		page = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(majorIndex) << (shiftForWord()))))));
		if (page == GIV(nilObj)) {
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
				longAtput((void *)(p),GIV(nilObj));
			}

			/* begin storePointer:ofObject:withValue: */
			assert(validStorePointerArgs(majorIndex, GIV(hiddenRootsObj), page));
			assert(isNonImmediate(GIV(hiddenRootsObj)));
			if (oopisGreaterThanOrEqualTo(GIV(hiddenRootsObj), GIV(oldSpaceStart))) {
				if (/* isYoung: */
					((!(page & (tagMask()))))
				 && (oopisLessThan(page, GIV(oldSpaceStart)))) {
					/* begin possibleRootStoreInto: */
					if (!((byteAt((void *)(GIV(hiddenRootsObj) + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
						remember(GIV(hiddenRootsObj));
					}
				}
			}

			/* most stores into young objects */
			longAtput((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(majorIndex) << (shiftForWord()))))),page);
			GIV(numClassTablePages) += 1;
			minorIndex = 0;
		}
		toDoLimit = (1U << (classTableMajorIndexShift())) - 1;
		for (i = minorIndex; i <= toDoLimit; i += 1) {
			if ((longAt((void *)((page + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))))) == GIV(nilObj)) {
				GIV(classTableIndex) = ((((usqInt)(majorIndex) << (classTableMajorIndexShift())))) + i;

				/* classTableIndex must never index the first page, which is reserved for classes known to the VM. */
				assert(GIV(classTableIndex) >= (1U << (classTableMajorIndexShift())));

				/* begin storePointer:ofObject:withValue: */
				assert(validStorePointerArgs(i, page, aBehavior));
				assert(isNonImmediate(page));
				if (oopisGreaterThanOrEqualTo(page, GIV(oldSpaceStart))) {
					if (/* isYoung: */
						((!(aBehavior & (tagMask()))))
					 && (oopisLessThan(aBehavior, GIV(oldSpaceStart)))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(page + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(page);
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((page + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),aBehavior);

				/* begin setHashBitsOf:to: */
				long32Atput((void *)(aBehavior + 4),((((long32At((void *)(aBehavior + 4))) | (identityHashHalfWordMask())) - (identityHashHalfWordMask()))) + (GIV(classTableIndex) & (identityHashHalfWordMask())));
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