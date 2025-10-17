/* Extracted from interp.c:33342 (function classString). */

	/* SpurMemoryManager>>#classString */

sqInt
classString(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassByteString) << (shiftForWord()))))));
}