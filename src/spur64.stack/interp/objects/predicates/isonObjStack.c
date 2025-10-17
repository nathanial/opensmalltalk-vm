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
  index = (fetchPointerofObject(ObjStackTopx, objStack)) +
          ObjStackNextx;
  while (index >= ObjStackFixedSlots) {
    if (oop == (fetchPointerofObject(index, objStack))) {
      return 1;
    }
    index -= 1;
  }
  nextPage =
      fetchPointerofObject(ObjStackNextx, objStack);
  if (nextPage) {
    if (isonObjStack(oop, nextPage)) {
      return 1;
    }
  }
  return 0;
}