/* Extracted from interp.c:32991 (function classAtIndex). */

/* SpurMemoryManager>>#classAtIndex: */

sqInt classAtIndex(sqInt classIndex) {
  sqInt classTablePage;
  sqInt fieldIndex;

  assert((classIndex >= 0) && ((classIndex <= (tagMask())) ||
                               ((classIndex >= (arrayClassIndexPun())) &&
                                (classIndex <= (classIndexMask())))));
  fieldIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());

  /* begin fetchPointer:ofObject: */
  classTablePage =
      longAt((void *)((hiddenRootsObj + BaseHeaderSize) +
                      ((((usqInt)(fieldIndex) << (shiftForWord()))))));
  if (classTablePage == nilObj) {
    return null;
  }
  fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);

  /* begin fetchPointer:ofObject: */
  return longAt((void *)((classTablePage + BaseHeaderSize) +
                         ((((usqInt)(fieldIndex) << (shiftForWord()))))));
}