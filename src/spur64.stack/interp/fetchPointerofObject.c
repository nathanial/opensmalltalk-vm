/* Extracted from interp.c:35076 (function fetchPointerofObject). */

sqInt
fetchPointerofObject(sqInt fieldIndex, sqInt objOop)
{
	return longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
}