/* Extracted from interp.c:33163 (function classFloat). */

sqInt
classFloat(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassFloat) << (shiftForWord()))))));
}