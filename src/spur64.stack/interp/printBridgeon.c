/* Extracted from interp.c:42652 (function printBridgeon). */

	/* SpurMemoryManager>>#printBridge:on: */

static NoDbgRegParms void
printBridgeon(sqInt oop, FILE *aStream)
{
	fprintf(aStream,
			"%p is a bridge hdr%d slot size %" PRIuSQINT "\n",
			((void *)oop),
			((byteAt((void *)(oop + (numSlotsFieldByteOffset())))) == (numSlotsMask())
			? 16
			: 8),
			numSlotsOfAny(oop));
}