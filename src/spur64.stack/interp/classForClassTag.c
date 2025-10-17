/* Extracted from interp.c:33188 (function classForClassTag). */

/*	No need to check this; classAtIndex: has a stricter assert:
	self assert: classIndex ~= self isForwardedObjectClassIndexPun. */

	/* SpurMemoryManager>>#classForClassTag: */

static NoDbgRegParms sqInt
classForClassTag(sqInt classIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classTablePage;
    sqInt fieldIndex;

	/* begin classAtIndex: */
	assert((classIndex >= 0)
	 && ((classIndex <= (tagMask()))
	 || ((classIndex >= (arrayClassIndexPun()))
	 && (classIndex <= (classIndexMask())))));
	fieldIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());

	/* begin fetchPointer:ofObject: */
	classTablePage = longAt((void *)((hiddenRootsObj + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
	if (classTablePage == nilObj) {
		return null;
	}
	fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);

	/* begin fetchPointer:ofObject: */
	return longAt((void *)((classTablePage + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
}