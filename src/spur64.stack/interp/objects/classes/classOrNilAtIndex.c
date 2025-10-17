/* Extracted from interp.c:33299 (function classOrNilAtIndex). */

/* SpurMemoryManager>>#classOrNilAtIndex: */

sqInt classOrNilAtIndex(sqInt classIndex) {
  sqInt classTablePage;
  sqInt fieldIndex;

  assert((classIndex <= (tagMask())) || (classIndex >= (arrayClassIndexPun())));
  fieldIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());

  classTablePage =
      fetchPointerofObject(fieldIndex, hiddenRootsObj);
  if (classTablePage == nilObj) {
    return nilObj;
  }
  fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);

  return fetchPointerofObject(fieldIndex, classTablePage);
}