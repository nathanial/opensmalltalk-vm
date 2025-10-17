/* Extracted from interp.c:33328 (function classSemaphore). */

	/* SpurMemoryManager>>#classSemaphore */

sqInt
classSemaphore(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassSemaphore) << (shiftForWord()))))));
}