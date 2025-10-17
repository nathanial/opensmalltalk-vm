/* Extracted from interp.c:32991 (function classAtIndex). */

/* SpurMemoryManager>>#classAtIndex: */

sqInt classAtIndex(sqInt classIndex) {
  sqInt classTablePage;
  sqInt fieldIndex;

  assert((classIndex >= 0) && ((classIndex <= (tagMask())) ||
                               ((classIndex >= (arrayClassIndexPun())) &&
                                (classIndex <= (classIndexMask())))));
  fieldIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());

  classTablePage = fetchPointerofObject(fieldIndex, hiddenRootsObj);
  if (classTablePage == nilObj) {
    return null;
  }
  fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);

  return fetchPointerofObject(fieldIndex, classTablePage);
}