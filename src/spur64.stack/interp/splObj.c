/* Extracted from interp.c:45681 (function splObj). */

sqInt
splObj(sqInt index)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))));
}