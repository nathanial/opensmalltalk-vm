/* Extracted from interp.c:33128 (function classExternalAddress). */

sqInt
classExternalAddress(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassExternalAddress) << (shiftForWord()))))));
}