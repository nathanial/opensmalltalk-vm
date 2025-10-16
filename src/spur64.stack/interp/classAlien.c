/* Extracted from interp.c:32977 (function classAlien). */

sqInt
classAlien(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassAlien) << (shiftForWord()))))));
}