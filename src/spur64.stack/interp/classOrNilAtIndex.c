/* Extracted from interp.c:33299 (function classOrNilAtIndex). */

sqInt
classOrNilAtIndex(sqInt classIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classTablePage;
    sqInt fieldIndex;

	assert((classIndex <= (tagMask()))
	 || (classIndex >= (arrayClassIndexPun())));
	fieldIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());

	/* begin fetchPointer:ofObject: */
	classTablePage = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
	if (classTablePage == GIV(nilObj)) {
		return GIV(nilObj);
	}
	fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);

	/* begin fetchPointer:ofObject: */
	return longAt((void *)((classTablePage + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
}