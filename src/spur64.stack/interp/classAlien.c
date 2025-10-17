/* Extracted from interp.c:32977 (function classAlien). */

	/* SpurMemoryManager>>#classAlien */

sqInt
classAlien(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassAlien) << (shiftForWord()))))));
}