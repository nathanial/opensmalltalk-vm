/* Extracted from interp.c:34119 (function displayObject). */

	/* SpurMemoryManager>>#displayObject */

sqInt
displayObject(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(TheDisplay) << (shiftForWord()))))));
}