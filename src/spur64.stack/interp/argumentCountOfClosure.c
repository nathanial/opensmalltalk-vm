/* Extracted from interp.c:50106 (function argumentCountOfClosure). */

sqInt
argumentCountOfClosure(sqInt closurePointer)
{
    sqInt oop;

	/* begin quickFetchInteger:ofObject: */
	oop = longAt((void *)((closurePointer + BaseHeaderSize) + ((((usqInt)(ClosureNumArgsIndex) << (shiftForWord()))))));
	assert((((oop) & 7) == 1));
	return (oop >> 3);
}