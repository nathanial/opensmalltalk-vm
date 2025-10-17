/* Extracted from interp.c:33018 (function classAtIndexput). */

/*	for become & GC of classes */

	/* SpurMemoryManager>>#classAtIndex:put: */

static NoDbgRegParms sqInt
classAtIndexput(sqInt classIndex, sqInt objOop)
{
    sqInt classTablePage;
    sqInt fieldIndex;

	assert((classIndex <= (tagMask()))
	 || (classIndex >= (arrayClassIndexPun())));
	assert((objOop == nilObj)
	 || ((((rawHashBitsOf(objOop)) == classIndex)
	 || ((classAtIndex(rawHashBitsOf(objOop))) == objOop))
	 && (objCouldBeClassObj(objOop))));
	fieldIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());

	/* begin fetchPointer:ofObject: */
	classTablePage = longAt((void *)((hiddenRootsObj + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
	if (classTablePage == nilObj) {
		error("attempt to add class to empty page");
	}
	fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);

	/* begin storePointer:ofObject:withValue: */
	assert(validStorePointerArgs(fieldIndex, classTablePage, objOop));
	assert(isNonImmediate(classTablePage));
	if (oopisGreaterThanOrEqualTo(classTablePage, oldSpaceStart)) {
		if (/* isYoung: */
			((!(objOop & (tagMask()))))
		 && (oopisLessThan(objOop, oldSpaceStart))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(classTablePage + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(classTablePage);
			}
		}
	}

	/* most stores into young objects */
	return longAtput((void *)((classTablePage + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))),objOop);
}