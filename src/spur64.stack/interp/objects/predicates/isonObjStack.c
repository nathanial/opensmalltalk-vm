/* Extracted from interp.c:37960 (function isonObjStack). */

/* SpurMemoryManager>>#is:onObjStack: */

static sqInt isonObjStack(sqInt oop, sqInt objStack) {
  sqInt index;
  sqInt nextPage;

  if (objStack == nilObj) {
    return 0;
  }
  assert((numSlotsOfAny(objStack)) == ObjStackPageSlots);

  /* There are four fixed slots in an obj stack, and a Topx of 0 indicates
     empty, so if there were 6 slots in an oop stack, full would be 2, and the
     last 0-rel index is 5. */
  index = (longAt((void *)((objStack + BaseHeaderSize) +
                           ((((usqInt)(ObjStackTopx) << (shiftForWord()))))))) +
          ObjStackNextx;
  while (index >= ObjStackFixedSlots) {
    if (oop == (longAt((void *)((objStack + BaseHeaderSize) +
                                ((((usqInt)(index) << (shiftForWord())))))))) {
      return 1;
    }
    index -= 1;
  }
  nextPage =
      longAt((void *)((objStack + BaseHeaderSize) +
                      ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
  if (nextPage) {
    if (isonObjStack(oop, nextPage)) {
      return 1;
    }
  }
  return 0;
}