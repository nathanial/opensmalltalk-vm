/* Extracted from interp.c:33163 (function classFloat). */

	/* SpurMemoryManager>>#classFloat */

sqInt
classFloat(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassFloat) << (shiftForWord()))))));
}