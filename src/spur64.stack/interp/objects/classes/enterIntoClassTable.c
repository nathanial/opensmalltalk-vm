/* Extracted from interp.c:34771 (function enterIntoClassTable). */

/*	Enter aBehavior into the class table and answer 0. Otherwise answer a
        primitive failure code. */

/* SpurMemoryManager>>#enterIntoClassTable: */

static sqInt enterIntoClassTable(sqInt aBehavior) {
  sqInt i;
  sqInt initialMajorIndex;
  sqInt majorIndex;
  sqInt minorIndex;
  usqInt p;
  sqInt page;
  sqInt toDoLimit;
  usqInt toDoLimitUsqInt;

  majorIndex = ((usqInt)(classTableIndex)) >> (classTableMajorIndexShift());
  initialMajorIndex = majorIndex;

  /* classTableIndex should never index the first page; it's reserved for known
   * classes */
  assert(initialMajorIndex > 0);
  minorIndex = classTableIndex & ((1U << (classTableMajorIndexShift())) - 1);
  while (1) {
    page = fetchPointerofObject(majorIndex, hiddenRootsObj);
    if (page == nilObj) {
      /* begin allocateSlotsInOldSpace:format:classIndex: */
      page = allocateSlotsInOldSpacebytesformatclassIndex(
          1U << (classTableMajorIndexShift()), /* objectBytesForSlots: */
          (1U << (classTableMajorIndexShift())
               ? ((((usqInt)((1U << (classTableMajorIndexShift())))
                    << (shiftForWord())))) +
                     (((1U << (classTableMajorIndexShift())) >= (numSlotsMask())
                           ? BaseHeaderSize + BaseHeaderSize
                           : BaseHeaderSize))
               : 8 /* allocationUnit */ + BaseHeaderSize),
          arrayFormat(), arrayClassIndexPun());
      if (!page) {
        return PrimErrNoMemory;
      }

      /* begin fillObj:numSlots:with: */
      assert(oopisLessThan(
          ((page + BaseHeaderSize) +
           ((1U << (classTableMajorIndexShift())) * BytesPerOop)) -
              1,
          addressAfter(page)));
      toDoLimitUsqInt =
          ((usqInt)(((page + BaseHeaderSize) +
                     ((1U << (classTableMajorIndexShift())) * BytesPerOop)) -
                    1));
      for (p = (((usqInt)(page + BaseHeaderSize))); p <= toDoLimitUsqInt;
           p += 8 /* allocationUnit */) {
        longAtput((void *)(p), nilObj);
      }

      storePointerofObjectwithValue(majorIndex, hiddenRootsObj, page);
      numClassTablePages += 1;
      minorIndex = 0;
    }
    toDoLimit = (1U << (classTableMajorIndexShift())) - 1;
    for (i = minorIndex; i <= toDoLimit; i += 1) {
      if ((fetchPointerofObject(i, page)) == nilObj) {
        classTableIndex =
            ((((usqInt)(majorIndex) << (classTableMajorIndexShift())))) + i;

        /* classTableIndex must never index the first page, which is reserved
         * for classes known to the VM. */
        assert(classTableIndex >= (1U << (classTableMajorIndexShift())));

        storePointerofObjectwithValue(i, page, aBehavior);

        /* begin setHashBitsOf:to: */
        long32Atput((void *)(aBehavior + 4),
                    ((((long32At((void *)(aBehavior + 4))) |
                       (identityHashHalfWordMask())) -
                      (identityHashHalfWordMask()))) +
                        (classTableIndex & (identityHashHalfWordMask())));
        assert((classAtIndex(rawHashBitsOf(aBehavior))) == aBehavior);
        return 0;
      }
    }
    majorIndex = ((((majorIndex + 1) & (classIndexMask())) < 1)
                      ? 1
                      : ((majorIndex + 1) & (classIndexMask())));
    if (majorIndex == initialMajorIndex) {
      return PrimErrLimitExceeded;
    }
  }
  return 0;
}