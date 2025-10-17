/* Extracted from interp.c:32984 (function classArray). */

	/* SpurMemoryManager>>#classArray */

sqInt
classArray(void)
{
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassArray) << (shiftForWord()))))));
}