/* Extracted from interp.c:40903 (function noCheckClassAtIndex). */

/* SpurMemoryManager>>#noCheckClassAtIndex: */

static sqInt noCheckClassAtIndex(sqInt classIndex) {
  sqInt classTablePage;
  sqInt fieldIndex;

  fieldIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());

  classTablePage =
      fetchPointerofObject(fieldIndex, hiddenRootsObj);
  if (classTablePage == nilObj) {
    return null;
  }
  fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);

  return fetchPointerofObject(fieldIndex, classTablePage);
}