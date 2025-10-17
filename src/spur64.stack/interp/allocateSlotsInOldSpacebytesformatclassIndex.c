/* Extracted from interp.c:31025 (function
 * allocateSlotsInOldSpacebytesformatclassIndex). */

/*	Answer the oop of a chunk of space in oldSpace with numSlots slots. The
        header will have been filled-in but not the contents. If no memory is
        available answer nil. */

/* SpurMemoryManager>>#allocateSlotsInOldSpace:bytes:format:classIndex: */

static NoDbgRegParms sqInt allocateSlotsInOldSpacebytesformatclassIndex(
    sqInt numSlots, usqInt totalBytes, sqInt formatField, sqInt classIndex) {
  sqInt chunk;

  if ((chunk = allocateOldSpaceChunkOfBytes(totalBytes))) {
    if (numSlots >= (numSlotsMask())) {
      long64Atput((void *)(chunk),
                  ((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) +
                      numSlots);
      long64Atput((void *)(chunk + BaseHeaderSize),
                  ((((((usqLong)(numSlotsMask()))) << (numSlotsFullShift()))) +
                   ((((usqInt)(formatField) << (formatShift()))))) +
                      classIndex);

      /* begin checkFreeSpace:ignoring: */
      assert(bitsSetInFreeSpaceMaskForAllFreeLists());
      assert(totalFreeOldSpace == (totalFreeListBytes()));
      if (((checkForLeaks & (GCCheckFreeSpace | GCModeNewSpace)) ==
           (GCCheckFreeSpace | GCModeNewSpace))) {
        runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace,
                                           chunk + BaseHeaderSize);
      }
      return chunk + BaseHeaderSize;
    }

    /* for header parsing we put a saturated slot count in the prepended
     * overflow size word */
    long64Atput((void *)(chunk),
                ((((((usqLong)numSlots)) << (numSlotsFullShift()))) +
                 ((((usqInt)(formatField) << (formatShift()))))) +
                    classIndex);

    /* begin checkFreeSpace:ignoring: */
    assert(bitsSetInFreeSpaceMaskForAllFreeLists());
    assert(totalFreeOldSpace == (totalFreeListBytes()));
    if (((checkForLeaks & (GCCheckFreeSpace | GCModeNewSpace)) ==
         (GCCheckFreeSpace | GCModeNewSpace))) {
      runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, chunk);
    }
    return chunk;
  }
  return 0;
}