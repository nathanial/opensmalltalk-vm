/* Extracted from interp.c:37445 (function isMarked). */

int
isMarked(sqInt objOop)
{
	return ((byteAt((void *)(objOop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0;
}