/* Extracted from interp.c:49117 (function bridgeFor). */

	/* SpurSegmentManager>>#bridgeFor: */

static NoDbgRegParms sqInt
bridgeFor(SpurSegmentInfo *aSegment)
{
    sqInt address;
    usqInt numSlots;

	address = (((aSegment->segSize)) + ((aSegment->segStart))) - (2 * BaseHeaderSize);

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
	return (numSlots == (numSlotsMask())
			? address + BaseHeaderSize
			: address);
}