/* Extracted from interp.c:32991 (function classAtIndex). */

sqInt
classAtIndex(sqInt classIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classTablePage;
    sqInt fieldIndex;

	assert((classIndex >= 0)
	 && ((classIndex <= (tagMask()))
	 || ((classIndex >= (arrayClassIndexPun()))
	 && (classIndex <= (classIndexMask())))));
	fieldIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());

	/* begin fetchPointer:ofObject: */
	classTablePage = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
	if (classTablePage == GIV(nilObj)) {
		return null;
	}
	fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);

	/* begin fetchPointer:ofObject: */
	return longAt((void *)((classTablePage + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
}