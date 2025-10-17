/* Extracted from interp.c:33335 (function classSmallInteger). */

	/* SpurMemoryManager>>#classSmallInteger */

sqInt
classSmallInteger(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassSmallInteger) << (shiftForWord()))))));
}