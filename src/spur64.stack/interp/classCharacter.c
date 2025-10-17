/* Extracted from interp.c:33077 (function classCharacter). */

	/* SpurMemoryManager>>#classCharacter */

sqInt
classCharacter(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassCharacter) << (shiftForWord()))))));
}