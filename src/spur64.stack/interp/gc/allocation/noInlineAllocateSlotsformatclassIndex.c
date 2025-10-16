/* Extracted from interp.c:40986 (function
 * noInlineAllocateSlotsformatclassIndex). */

/*	Allocate an object with numSlots space. If there is room beneath
        scavengeThreshold allocate in newSpace, otherwise alocate in oldSpace.
   If there is not room in newSpace and a scavenge is not already scheduled,
   schedule a scavenge. */

/* SpurMemoryManager>>#noInlineAllocateSlots:format:classIndex: */

static NeverInline sqInt noInlineAllocateSlotsformatclassIndex(
    sqInt numSlots, sqInt formatField, sqInt classIndex) {
  usqInt newObj;
  usqInt numBytes;

  /* begin allocateSlots:format:classIndex: */
  if (numSlots >= (numSlotsMask())) {
    if ((((usqInt)(numSlots)) >> 56) > 0) {
      return null;
    }
    newObj = freeStart + BaseHeaderSize;
    numBytes = (BaseHeaderSize + BaseHeaderSize) + (numSlots * BytesPerOop);
  } else {
    newObj = freeStart;
    numBytes = BaseHeaderSize + ((numSlots < 1 ? 8 /* allocationUnit */
                                               : numSlots * BytesPerOop));
  }
  if ((freeStart + numBytes) > scavengeThreshold) {
    if (numSlots <= ((1U << (fixedFieldsFieldWidth())) - 1)) {
      if (!needGCFlag) {
        /* begin scheduleScavenge */
        needGCFlag = 1;
        forceInterruptCheck();
      }
    }
    return allocateSlotsInOldSpacebytesformatclassIndex(
        numSlots, numBytes, formatField, classIndex);
  }
  if (numSlots >= (numSlotsMask())) {
    longAtput((void *)(freeStart),
              ((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) +
                  numSlots);
    longAtput((void *)(newObj),
              ((((((usqLong)(numSlotsMask()))) << (numSlotsFullShift()))) +
               ((((usqInt)(formatField) << (formatShift()))))) +
                  classIndex);
  } else {
    longAtput((void *)(newObj),
              ((((((usqLong)numSlots)) << (numSlotsFullShift()))) +
               ((((usqInt)(formatField) << (formatShift()))))) +
                  classIndex);
  }

  /* for header parsing we put a saturated slot count in the prepended overflow
   * size word */
  assert((numBytes % (allocationUnit())) == 0);
  assert((newObj % (allocationUnit())) == 0);
  freeStart += numBytes;
  return newObj;
}