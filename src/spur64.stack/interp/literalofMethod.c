/* Extracted from interp.c:55569 (function literalofMethod). */

sqInt
literalofMethod(sqInt offset, sqInt methodPointer)
{
	return longAt((void *)((methodPointer + BaseHeaderSize) + ((((usqInt)((offset + LiteralStart)) << (shiftForWord()))))));
}