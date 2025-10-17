/* Extracted from interp.c:42537 (function primitiveErrorTable). */

	/* SpurMemoryManager>>#primitiveErrorTable */

sqInt
primitiveErrorTable(void)
{
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(PrimitiveErrorTableIndex) << (shiftForWord()))))));
}