/* Extracted from interp.c:27068 (function hackSlimBridgeToat). */

/*	This is a horrible hack for getting to the first object in eden if
        pastSpace is almost full.
        If there is only one (64-bit) word at the end of pastSpace there is no
        room for a full
        bridge, but there is room for this hack. */

/* Spur64BitMemoryManager>>#hackSlimBridgeTo:at: */

static NoDbgRegParms void hackSlimBridgeToat(sqInt objOop, sqInt startAddress) {
  assert(oopisLessThan(startAddress, objOop));
  assert(oopisLessThanOrEqualTo(objOop, oldSpaceStart));
  assert(
      ((startAddress + (allocationUnit())) == objOop) ||
      (((startAddress + (allocationUnit())) + (allocationUnit())) == objOop));
  longAtput(
      (void *)(startAddress),
      ((startAddress + 8 /* allocationUnit */) == objOop
           ? ((sqInt)((usqInt)((numSlotsMask())) << (numSlotsFullShift())))
           : ((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) + 1));
}