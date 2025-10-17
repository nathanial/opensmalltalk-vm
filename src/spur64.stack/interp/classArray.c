/* Extracted from interp.c:32984 (function classArray). */

	/* SpurMemoryManager>>#classArray */

sqInt
classArray(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassArray) << (shiftForWord()))))));
}