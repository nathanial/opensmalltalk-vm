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

  classTablePage = fetchPointerofObject(fieldIndex, hiddenRootsObj);
  if (classTablePage == nilObj) {
    classOrNil = nilObj;
    goto l1;
  }
  fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);
  classOrNil = fetchPointerofObject(fieldIndex, classTablePage);
  /* end classOrNilAtIndex: */
l1:
  return (classOrNil != nilObj) &&
         (((long32At((void *)(classOrNil + 4))) &
           (identityHashHalfWordMask())) == classIndex);
}