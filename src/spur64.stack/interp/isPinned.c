/* Extracted from interp.c:37543 (function isPinned). */

int
isPinned(sqInt objOop)
{
	return ((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0;
}