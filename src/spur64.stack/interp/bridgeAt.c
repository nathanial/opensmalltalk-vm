/* Extracted from interp.c:49102 (function bridgeAt). */

/* SpurSegmentManager>>#bridgeAt: */

static NoDbgRegParms sqInt bridgeAt(sqInt segIndex) {
  sqInt address;
  usqInt numSlots;

  /* begin bridgeFor: */
  address = (((((&(segments[segIndex])))->segSize)) +
             ((((&(segments[segIndex])))->segStart))) -
            (2 * BaseHeaderSize);
  numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
  return (numSlots == (numSlotsMask()) ? address + BaseHeaderSize : address);
}