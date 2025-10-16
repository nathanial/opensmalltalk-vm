/* Extracted from interp.c:61439 (function quickFetchIntegerofObject). */

static NoDbgRegParms sqInt
quickFetchIntegerofObject(sqInt fieldIndex, sqInt objectPointer)
{
    sqInt oop;

	oop = longAt((void *)((objectPointer + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
	assert((((oop) & 7) == 1));
	return (oop >> 3);
}