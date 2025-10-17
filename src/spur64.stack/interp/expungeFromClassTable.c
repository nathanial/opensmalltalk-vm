/* Extracted from interp.c:34951 (function expungeFromClassTable). */

/*	Remove aBehavior from the class table. */

/* SpurMemoryManager>>#expungeFromClassTable: */

static void expungeFromClassTable(sqInt aBehavior) {
  sqInt classIndex;
  sqInt classTablePage;
  sqInt majorIndex;
  sqInt minorIndex;

  assert(isInClassTable(aBehavior));
  classIndex =
      (long32At((void *)(aBehavior + 4))) & (identityHashHalfWordMask());
  majorIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());
  minorIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);
  classTablePage =
      longAt((void *)((hiddenRootsObj + BaseHeaderSize) +
                      ((((usqInt)(majorIndex) << (shiftForWord()))))));
  assert(classTablePage != classTableFirstPage);
  assert((numSlotsOf(classTablePage)) == (classTablePageSize()));
  assert((fetchPointerofObject(minorIndex, classTablePage)) == aBehavior);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(classTablePage)) && (!(isForwarded(classTablePage))));
  assert(validStorePointerUncheckedArgs(minorIndex, classTablePage, nilObj));
  longAtput((void *)((classTablePage + BaseHeaderSize) +
                     ((((usqInt)(minorIndex) << (shiftForWord()))))),
            nilObj);

  /* If the removed class is before the classTableIndex, set the
     classTableIndex to point to the empty slot so as to reuse it asap. */
  if (classIndex < classTableIndex) {
    classTableIndex = classIndex;
  }

  /* classTableIndex must never index the first page, which is reserved for
   * classes known to the VM. */
  assert(classTableIndex >= (1U << (classTableMajorIndexShift())));
}