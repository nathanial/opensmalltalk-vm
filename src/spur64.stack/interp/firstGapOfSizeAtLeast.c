/* Extracted from interp.c:49205 (function firstGapOfSizeAtLeast). */

/*	Answer the segment limit of the first segment followed by a gap of at
	least size bytes.
 */

	/* SpurSegmentManager>>#firstGapOfSizeAtLeast: */

static NoDbgRegParms void *
firstGapOfSizeAtLeast(sqInt size)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    sqInt bridge;
    sqInt i;
    usqInt numSlots;

	for (i = 0; i <= (numSegments - 2); i += 1) {
		/* begin bridgeAt: */
		/* begin bridgeFor: */
		address = (((((&(segments[i])))->segSize)) + ((((&(segments[i])))->segStart))) - (2 * BaseHeaderSize);
		numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
		bridge = (numSlots == (numSlotsMask())
					? address + BaseHeaderSize
					: address);
		if (((bytesInBody(bridge)) - (2 * BaseHeaderSize)) >= size) {
			return ((void *)((((segments[i]).segSize)) + (((segments[i]).segStart))));
		}
	}
	return ((void *)((((segments[numSegments - 1]).segSize)) + (((segments[numSegments - 1]).segStart))));
}