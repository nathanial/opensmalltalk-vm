/* Extracted from interp.c:37224 (function isCopiedIntoSegment). */

static NoDbgRegParms int
isCopiedIntoSegment(sqInt anObjectInTheHeap)
{
	return ((byteAt((void *)(anObjectInTheHeap + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0;
}