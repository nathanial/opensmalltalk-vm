/* Extracted from interp.c:58280 (function penultimateLiteralOf). */

	/* StackInterpreter>>#penultimateLiteralOf: */

static NoDbgRegParms sqInt
penultimateLiteralOf(sqInt aMethodOop)
{
    sqInt offset;

	assert(isOopCompiledMethod(aMethodOop));
	offset = (literalCountOf(aMethodOop)) - 2;

	/* begin literal:ofMethod: */
	return longAt((void *)((aMethodOop + BaseHeaderSize) + ((((usqInt)((offset + LiteralStart)) << (shiftForWord()))))));
}