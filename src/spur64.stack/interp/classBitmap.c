/* Extracted from interp.c:33058 (function classBitmap). */

	/* SpurMemoryManager>>#classBitmap */

sqInt
classBitmap(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassBitmap) << (shiftForWord()))))));
}