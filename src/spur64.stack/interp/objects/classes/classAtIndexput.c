/* Extracted from interp.c:33018 (function classAtIndexput). */

/*	for become & GC of classes */

/* SpurMemoryManager>>#classAtIndex:put: */

static sqInt classAtIndexput(sqInt classIndex, sqInt objOop) {
  sqInt classTablePage;
  sqInt fieldIndex;

  assert((classIndex <= (tagMask())) || (classIndex >= (arrayClassIndexPun())));
  assert((objOop == nilObj) ||
         ((((rawHashBitsOf(objOop)) == classIndex) ||
           ((classAtIndex(rawHashBitsOf(objOop))) == objOop)) &&
          (objCouldBeClassObj(objOop))));
  fieldIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());

  classTablePage = fetchPointerofObject(fieldIndex, hiddenRootsObj);
  if (classTablePage == nilObj) {
    error("attempt to add class to empty page");
  }
  fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);

  return storePointerofObjectwithValue(fieldIndex, classTablePage, objOop);
}