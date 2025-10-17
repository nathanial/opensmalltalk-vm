/* Extracted from interp.c:49229 (function isEmptySegment). */

/* SpurSegmentManager>>#isEmptySegment: */

static sqInt isEmptySegment(SpurSegmentInfo *seg) {
  sqInt address;
  sqInt firstObj;
  usqInt numSlots;

  address = (seg->segStart);

  /* begin objectStartingAt: */
  numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
  firstObj =
      (numSlots == (numSlotsMask()) ? address + BaseHeaderSize : address);
  return (((longAt((void *)(firstObj))) & (classIndexMask())) ==
          (isFreeObjectClassIndexPun())) &&
         ((addressAfter(firstObj)) ==
          ((((seg->segSize)) + ((seg->segStart))) - (2 * BaseHeaderSize)));
}