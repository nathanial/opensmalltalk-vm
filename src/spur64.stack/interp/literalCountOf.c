/* Extracted from interp.c:38215 (function literalCountOf). */

	/* SpurMemoryManager>>#literalCountOf: */

usqInt
literalCountOf(sqInt methodPointer)
{
    sqInt header;

	/* begin methodHeaderOf: */
	assert(isCompiledMethod(methodPointer));
	header = longAt((void *)((methodPointer + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

	/* begin literalCountOfMethodHeader: */
	assert((((header) & 7) == 1));
	return ((header >> 3)) & AlternateHeaderNumLiteralsMask;
}