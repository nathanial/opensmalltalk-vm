/* Extracted from interp.c:27123 (function initSegmentBridgeWithBytesat). */

static NoDbgRegParms void
initSegmentBridgeWithBytesat(usqLong numBytes, sqInt address)
{
    sqLong numSlots;

	assert(((numBytes % (allocationUnit())) == 0)
	 && (numBytes >= (BaseHeaderSize + BaseHeaderSize)));
	numSlots = ((usqLong)(((numBytes - BaseHeaderSize) - BaseHeaderSize))) >> (shiftForWord());
	if (numSlots) {
		longAtput((void *)(address),((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) + numSlots);
		longAtput((void *)(address + BaseHeaderSize),(((((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) + (1U << (pinnedBitShift()))) + (1ULL << (markedBitFullShift()))) + ((((usqInt)((sixtyFourBitIndexableFormat())) << (formatShift()))))) + (segmentBridgePun()));
	}
	else {
		longAtput((void *)(address),(((1U << (pinnedBitShift())) + (1ULL << (markedBitFullShift()))) + ((((usqInt)((sixtyFourBitIndexableFormat())) << (formatShift()))))) + (segmentBridgePun()));
	}
}