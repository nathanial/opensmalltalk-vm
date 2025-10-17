/* Extracted from interp.c:27123 (function initSegmentBridgeWithBytesat). */

/*	Must have room for a double header or a short object with the forwarding
        slot (16 bytes either way).
 */

/* Spur64BitMemoryManager>>#initSegmentBridgeWithBytes:at: */

static void initSegmentBridgeWithBytesat(usqLong numBytes,
                                                       sqInt address) {
  sqLong numSlots;

  assert(((numBytes % (allocationUnit())) == 0) &&
         (numBytes >= (BaseHeaderSize + BaseHeaderSize)));
  numSlots = ((usqLong)(((numBytes - BaseHeaderSize) - BaseHeaderSize))) >>
             (shiftForWord());
  if (numSlots) {
    longAtput((void *)(address),
              ((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) +
                  numSlots);
    longAtput(
        (void *)(address + BaseHeaderSize),
        (((((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) +
           (1U << (pinnedBitShift()))) +
          (1ULL << (markedBitFullShift()))) +
         ((((usqInt)((sixtyFourBitIndexableFormat())) << (formatShift()))))) +
            (segmentBridgePun()));
  } else {
    longAtput(
        (void *)(address),
        (((1U << (pinnedBitShift())) + (1ULL << (markedBitFullShift()))) +
         ((((usqInt)((sixtyFourBitIndexableFormat())) << (formatShift()))))) +
            (segmentBridgePun()));
  }
}