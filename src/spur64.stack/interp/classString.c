/* Extracted from interp.c:33342 (function classString). */

sqInt
classString(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassByteString) << (shiftForWord()))))));
}