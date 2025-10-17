/* Extracted from interp.c:33149 (function classExternalLibrary). */

	/* SpurMemoryManager>>#classExternalLibrary */

sqInt
classExternalLibrary(void)
{
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassExternalLibrary) << (shiftForWord()))))));
}