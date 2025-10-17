/* Extracted from interp.c:33299 (function classOrNilAtIndex). */

/* SpurMemoryManager>>#classOrNilAtIndex: */

sqInt classOrNilAtIndex(sqInt classIndex) {
  sqInt classTablePage;
  sqInt fieldIndex;

  assert((classIndex <= (tagMask())) || (classIndex >= (arrayClassIndexPun())));
  fieldIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());

  /* begin fetchPointer:ofObject: */
  classTablePage =
      longAt((void *)((hiddenRootsObj + BaseHeaderSize) +
                      ((((usqInt)(fieldIndex) << (shiftForWord()))))));
  if (classTablePage == nilObj) {
    return nilObj;
  }
  fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);

  /* begin fetchPointer:ofObject: */
  return longAt((void *)((classTablePage + BaseHeaderSize) +
                         ((((usqInt)(fieldIndex) << (shiftForWord()))))));
}