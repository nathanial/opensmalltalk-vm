/* Extracted from interp.c:52964 (function fetchFloatofObject). */

double
fetchFloatofObject(sqInt fieldIndex, sqInt objectPointer)
{
    sqInt floatOop;

	floatOop = longAt((void *)((objectPointer + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
	return floatValueOf(floatOop);
}