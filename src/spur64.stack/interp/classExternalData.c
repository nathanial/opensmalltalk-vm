/* Extracted from interp.c:33135 (function classExternalData). */

sqInt
classExternalData(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassExternalData) << (shiftForWord()))))));
}