/* Extracted from interp.c:32984 (function classArray). */

sqInt
classArray(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassArray) << (shiftForWord()))))));
}