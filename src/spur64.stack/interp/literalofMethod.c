/* Extracted from interp.c:55569 (function literalofMethod). */

	/* StackInterpreter>>#literal:ofMethod: */

sqInt
literalofMethod(sqInt offset, sqInt methodPointer)
{
	return longAt((void *)((methodPointer + BaseHeaderSize) + ((((usqInt)((offset + LiteralStart)) << (shiftForWord()))))));
}