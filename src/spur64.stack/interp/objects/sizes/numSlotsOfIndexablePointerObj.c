/* Extracted from interp.c:41215 (function numSlotsOfIndexablePointerObj). */

/*	Answer the number of strong pointer fields in the given indexable
   pointer object.
 */

/* SpurMemoryManager>>#numSlotsOfIndexablePointerObj: */

static usqInt numSlotsOfIndexablePointerObj(sqInt objOop) {

  assert((formatOf(objOop)) == (indexablePointersFormat()));
  if (((longAt((void *)(objOop))) & (classIndexMask())) ==
      ClassMethodContextCompactIndex) {
    return CtxtTempFrameStart + (fetchStackPointerOf(objOop));
  } else {

    return numSlotsOf(objOop);
  }
}
