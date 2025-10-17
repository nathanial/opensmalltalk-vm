/* Extracted from interp.c:34119 (function displayObject). */

	/* SpurMemoryManager>>#displayObject */

sqInt
displayObject(void)
{
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(TheDisplay) << (shiftForWord()))))));
}