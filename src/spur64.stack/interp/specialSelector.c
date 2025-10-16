/* Extracted from interp.c:63815 (function specialSelector). */

sqInt
specialSelector(sqInt index)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	/* begin fetchPointer:ofObject: */
	return longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SpecialSelectors) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)((index * 2)) << (shiftForWord()))))));
}