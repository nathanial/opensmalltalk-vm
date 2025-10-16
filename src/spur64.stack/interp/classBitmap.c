/* Extracted from interp.c:33058 (function classBitmap). */

sqInt
classBitmap(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassBitmap) << (shiftForWord()))))));
}