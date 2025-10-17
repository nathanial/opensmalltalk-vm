/* Extracted from interp.c:40903 (function noCheckClassAtIndex). */

	/* SpurMemoryManager>>#noCheckClassAtIndex: */

static NoDbgRegParms sqInt
noCheckClassAtIndex(sqInt classIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classTablePage;
    sqInt fieldIndex;

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