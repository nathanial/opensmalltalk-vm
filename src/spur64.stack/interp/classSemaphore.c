/* Extracted from interp.c:33328 (function classSemaphore). */

	/* SpurMemoryManager>>#classSemaphore */

sqInt
classSemaphore(void)
{
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassSemaphore) << (shiftForWord()))))));
}