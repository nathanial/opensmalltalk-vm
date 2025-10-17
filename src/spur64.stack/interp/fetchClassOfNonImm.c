/* Extracted from interp.c:34999 (function fetchClassOfNonImm). */

/* SpurMemoryManager>>#fetchClassOfNonImm: */

static NoDbgRegParms NeverInline sqInt fetchClassOfNonImm(sqInt objOop) {
  sqInt classIndex;
  sqInt classTablePage;
  sqInt fieldIndex;

  classIndex = (longAt((void *)(objOop))) & (classIndexMask());
  if (classIndex <= (classIsItselfClassIndexPun())) {
    if (classIndex == (classIsItselfClassIndexPun())) {
      return objOop;
    }

    /* Answer nil to avoid the assert failure in classOrNilAtIndex: */
    if (classIndex == (isForwardedObjectClassIndexPun())) {
      return nilObj;
    }
  }
  assert(classIndex >= (arrayClassIndexPun()));

  /* begin classOrNilAtIndex: */
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