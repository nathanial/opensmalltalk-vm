/* Extracted from interp.c:37656 (function isValidClassTag). */

/* SpurMemoryManager>>#isValidClassTag: */

sqInt isValidClassTag(sqInt classIndex) {
  sqInt classOrNil;
  sqInt classTablePage;
  sqInt fieldIndex;

  assert(((classIndex >= 0) &&
          (classIndex <= ((1U << (classIndexFieldWidth())) - 1))));
  if (((classIndex >= ((tagMask()) + 1)) &&
       (classIndex <= (lastClassIndexPun())))) {
    return 0;
  }

  /* begin classOrNilAtIndex: */
  assert((classIndex <= (tagMask())) || (classIndex >= (arrayClassIndexPun())));
  fieldIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());

  /* begin fetchPointer:ofObject: */
  classTablePage =
      longAt((void *)((hiddenRootsObj + BaseHeaderSize) +
                      ((((usqInt)(fieldIndex) << (shiftForWord()))))));
  if (classTablePage == nilObj) {
    classOrNil = nilObj;
    goto l1;
  }
  fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);
  classOrNil = longAt((void *)((classTablePage + BaseHeaderSize) +
                               ((((usqInt)(fieldIndex) << (shiftForWord()))))));
  /* end classOrNilAtIndex: */
l1:
  return (classOrNil != nilObj) &&
         (((long32At((void *)(classOrNil + 4))) &
           (identityHashHalfWordMask())) == classIndex);
}