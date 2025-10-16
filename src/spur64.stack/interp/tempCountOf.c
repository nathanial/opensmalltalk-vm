/* Extracted from interp.c:65111 (function tempCountOf). */

usqInt
tempCountOf(sqInt methodPointer)
{
    sqInt header;

	/* begin methodHeaderOf: */
	assert(isCompiledMethod(methodPointer));
	header = longAt((void *)((methodPointer + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

	/* begin temporaryCountOfMethodHeader: */
	return (((usqInt)(header)) >> MethodHeaderTempCountShift) & 0x3F;
}