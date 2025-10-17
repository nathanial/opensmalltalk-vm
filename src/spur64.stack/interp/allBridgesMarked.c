/* Extracted from interp.c:49054 (function allBridgesMarked). */

	/* SpurSegmentManager>>#allBridgesMarked */

static sqInt
allBridgesMarked(void)
{
    sqInt address;
    sqInt bridgeObj;
    sqInt i;
    usqInt numSlots;

	for (i = 0; i < numSegments; i += 1) {
		/* begin bridgeAt: */
		/* begin bridgeFor: */
		address = (((((&(segments[i])))->segSize)) + ((((&(segments[i])))->segStart))) - (2 * BaseHeaderSize);
		numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
		bridgeObj = (numSlots == (numSlotsMask())
					? address + BaseHeaderSize
					: address);
		assert(isValidSegmentBridge(bridgeObj));
		if (!((byteAt((void *)(bridgeObj + (markBitsByteOffset())))) & (1U << (markedBitByteShift())))) {
			return 0;
		}
	}
	return 1;
}