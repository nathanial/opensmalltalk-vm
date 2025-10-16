/* Extracted from interp.c:42537 (function primitiveErrorTable). */

sqInt
primitiveErrorTable(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(PrimitiveErrorTableIndex) << (shiftForWord()))))));
}