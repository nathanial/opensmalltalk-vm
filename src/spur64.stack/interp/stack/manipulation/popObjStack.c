/* Extracted from interp.c:42359 (function popObjStack). */

/* SpurMemoryManager>>#popObjStack: */

static sqInt popObjStack(sqInt objStack) {
  sqInt myx;
  sqInt nextPage;
  sqInt top;
  sqInt topx;

  eassert(isValidObjStack(objStack));
  topx = fetchPointerofObject(ObjStackTopx, objStack);
  if (!topx) {
    assert((fetchPointerofObject(ObjStackNextx, objStack)) == 0);

    /* for debugging markAndTrace: set (MarkStackRecord := OrderedCollection
     * new) */
    return null;
  }
  topx -= 1;
  top = longAt((
      void *)((objStack + BaseHeaderSize) +
              ((((usqInt)((topx + ObjStackFixedSlots)) << (shiftForWord()))))));
  assert(addressCouldBeOop(top));

  /* for debugging markAndTrace: set (MarkStackRecord := OrderedCollection new)
   */

  /* begin storePointer:ofObjStack:withValue: */
  assert((formatOf(objStack)) == (wordIndexableFormat()));
  longAtput((void *)((objStack + BaseHeaderSize) +
                     ((((usqInt)(ObjStackTopx) << (shiftForWord()))))),
            topx);
  if ((topx == 0) &&
      (((nextPage = fetchPointerofObject(ObjStackNextx, objStack))) != 0)) {
    /* begin storePointer:ofObjStack:withValue: */
    assert((formatOf(nextPage)) == (wordIndexableFormat()));
    longAtput((void *)((nextPage + BaseHeaderSize) +
                       ((((usqInt)(ObjStackFreex) << (shiftForWord()))))),
              objStack);

    /* begin storePointer:ofObjStack:withValue: */
    assert((formatOf(objStack)) == (wordIndexableFormat()));
    longAtput((void *)((objStack + BaseHeaderSize) +
                       ((((usqInt)(ObjStackNextx) << (shiftForWord()))))),
              0);
    myx = fetchPointerofObject(ObjStackMyx, objStack);

    /* begin updateRootOfObjStackAt:with: */
    storePointerofObjectwithValue(myx, hiddenRootsObj, nextPage);
    switch (myx) {
    case MarkStackRootIndex:
      markStack = nextPage;
      break;
    case WeaklingStackRootIndex:
      weaklingStack = nextPage;
      break;
    case MournQueueRootIndex:
      mournQueue = nextPage;
      break;
    default:
      error("Case not found and no otherwise clause");
    }
    eassert(isValidObjStack(nextPage));
  } else {
    eassert(isValidObjStack(objStack));
  }
  return top;
}