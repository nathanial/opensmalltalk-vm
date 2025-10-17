/* Extracted from interp.c:33188 (function classForClassTag). */

/*	No need to check this; classAtIndex: has a stricter assert:
        self assert: classIndex ~= self isForwardedObjectClassIndexPun. */

/* SpurMemoryManager>>#classForClassTag: */

static sqInt classForClassTag(sqInt classIndex) {
  sqInt classTablePage;
  sqInt fieldIndex;

  /* begin classAtIndex: */
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