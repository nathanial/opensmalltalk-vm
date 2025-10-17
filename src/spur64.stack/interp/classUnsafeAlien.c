/* Extracted from interp.c:33418 (function classUnsafeAlien). */

	/* SpurMemoryManager>>#classUnsafeAlien */

sqInt
classUnsafeAlien(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassUnsafeAlien) << (shiftForWord()))))));
}