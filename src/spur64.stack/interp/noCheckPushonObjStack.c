/* Extracted from interp.c:40926 (function noCheckPushonObjStack). */

/*	Push an element on an objStack. Split from push:onObjStack: for testing.
 */

/* SpurMemoryManager>>#noCheckPush:onObjStack: */

static NoDbgRegParms sqInt noCheckPushonObjStack(sqInt objOop, sqInt objStack) {
  sqInt topx;

  eassert(isValidObjStack(objStack));

  /* for debugging markAndTrace: set (MarkStackRecord := OrderedCollection new)
   */
  topx = longAt((void *)((objStack + BaseHeaderSize) +
                         ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
  if (topx >= ObjStackLimit) {
    noCheckPushonObjStack(
        objOop, ensureRoomOnObjStackAt(longAt((
                    void *)((objStack + BaseHeaderSize) +
                            ((((usqInt)(ObjStackMyx) << (shiftForWord()))))))));
  } else {
    /* begin storePointer:ofObjStack:withValue: */
    assert((formatOf(objStack)) == (wordIndexableFormat()));
    longAtput((void *)((objStack + BaseHeaderSize) +
                       ((((usqInt)((ObjStackFixedSlots + topx))
                          << (shiftForWord()))))),
              objOop);

    /* begin storePointer:ofObjStack:withValue: */
    assert((formatOf(objStack)) == (wordIndexableFormat()));
    longAtput((void *)((objStack + BaseHeaderSize) +
                       ((((usqInt)(ObjStackTopx) << (shiftForWord()))))),
              topx + 1);
  }
  return objOop;
}