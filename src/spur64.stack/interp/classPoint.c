/* Extracted from interp.c:33321 (function classPoint). */

sqInt
classPoint(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassPoint) << (shiftForWord()))))));
}