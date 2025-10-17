/* Extracted from interp.c:41601 (function objectStartingAt). */

/*	For enumerating objects find the header of the first object in a space.
        If the object starts with an overflow size field it will start at the
   next allocationUnit. c.f. numSlotsOf: */

/* SpurMemoryManager>>#objectStartingAt: */

static NoDbgRegParms sqInt objectStartingAt(sqInt address) {
  usqInt numSlots;

  numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
  return (numSlots == (numSlotsMask()) ? address + BaseHeaderSize : address);
}