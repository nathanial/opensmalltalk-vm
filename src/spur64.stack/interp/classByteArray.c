/* Extracted from interp.c:33070 (function classByteArray). */

sqInt
classByteArray(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassByteArray) << (shiftForWord()))))));
}