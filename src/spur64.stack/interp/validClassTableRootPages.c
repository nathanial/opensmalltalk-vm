/* Extracted from interp.c:46724 (function validClassTableRootPages). */

static sqInt
validClassTableRootPages(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    usqInt numSlots;
    sqInt obj;
    sqInt toDoLimit;

	if (!(((/* begin numSlotsOf: */
			assert((classIndexOf(GIV(hiddenRootsObj))) > (isForwardedObjectClassIndexPun())),
		(((numSlots = byteAt((void *)(GIV(hiddenRootsObj) + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(GIV(hiddenRootsObj) - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots))) == ((1U << (22 /* classIndexFieldWidth */ - (classTableMajorIndexShift()))) + 8 /* hiddenRootSlots */))) {
		return 0;
	}

	/* is it in range? */
	if (!((GIV(numClassTablePages) > 1)
		 && (GIV(numClassTablePages) <= (1U << (22 /* classIndexFieldWidth */ - (classTableMajorIndexShift())))))) {
		return 0;
	}

	/* are all pages the right size? */
	for (i = 0; i < GIV(numClassTablePages); i += 1) {
		obj = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
		if (!((addressCouldBeObj(obj))
			 && (((/* begin numSlotsOf: */
				assert((classIndexOf(obj)) > (isForwardedObjectClassIndexPun())),
			(((numSlots = byteAt((void *)(obj + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
						? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(obj - BaseHeaderSize)))) << 8)))))) >> 8
						: numSlots))) == (1U << (classTableMajorIndexShift()))))) {
			return 0;
		}
	}

	/* are all entries beyond numClassTablePages nil? */
	toDoLimit = (1U << (22 /* classIndexFieldWidth */ - (classTableMajorIndexShift()))) - 1;
	for (i = GIV(numClassTablePages); i <= toDoLimit; i += 1) {
		if ((longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))))) != GIV(nilObj)) {
			return 0;
		}
	}
	return 1;
}