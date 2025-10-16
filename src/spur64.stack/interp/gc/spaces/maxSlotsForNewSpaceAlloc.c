/* Extracted from interp.c:40679 (function maxSlotsForNewSpaceAlloc). */

/*	Almost entirely arbitrary, but we dont want 1Mb bitmaps allocated in
   eden. But this choice means no check for numSlots > maxSlotsForNewSpaceAlloc
        for non-variable allocations. */

/* SpurMemoryManager>>#maxSlotsForNewSpaceAlloc */

sqInt maxSlotsForNewSpaceAlloc(void) {
  return (1U << (fixedFieldsFieldWidth())) - 1;
}