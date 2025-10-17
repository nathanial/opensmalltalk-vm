/* Extracted from interp.c:33142 (function classExternalFunction). */

	/* SpurMemoryManager>>#classExternalFunction */

sqInt
classExternalFunction(void)
{
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassExternalFunction) << (shiftForWord()))))));
}