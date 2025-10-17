/* Extracted from interp.c:39021 (function mapMournQueue). */

/* SpurMemoryManager>>#mapMournQueue */

static NeverInline void mapMournQueue(void) {
  sqInt iSqInt;
  sqInt mourner;
  sqInt numOnThisPage;
  sqInt objStackPage;
  sqInt referent;

  /* begin objStack:do: */
  if (mournQueue == nilObj) {
    goto l1;
  }
  eassert(isValidObjStack(mournQueue));
  objStackPage = mournQueue;
  while (objStackPage != 0) {
    numOnThisPage =
        longAt((void *)((objStackPage + BaseHeaderSize) +
                        ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
    for (iSqInt = ((numOnThisPage + ObjStackFixedSlots) - 1);
         iSqInt >= ObjStackFixedSlots; iSqInt += -1) {
      mourner = longAt((void *)((objStackPage + BaseHeaderSize) +
                                ((((usqInt)(iSqInt) << (shiftForWord()))))));
      if ((!(mourner & (tagMask())))) {
        if ((!((longAt((void *)(mourner))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          /* begin followForwarded: */
          assert(isUnambiguouslyForwarder(mourner));
          referent = longAt(
              (void *)((mourner + BaseHeaderSize) + (0U << (shiftForWord()))));
          while (isOopForwarded(referent)) {
            referent = longAt((void *)((referent + BaseHeaderSize) +
                                       (0U << (shiftForWord()))));
          }
          mourner = referent;
        }
        if (!(isScavengeSurvivor(mourner))) {
          mourner = copyAndForwardMourner(mourner);
        }

        /* we could check for change but writes are cheap with write buffers...
         */

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(objStackPage)) &&
               (!(isForwarded(objStackPage))));
        assert(validStorePointerUncheckedArgs(iSqInt, objStackPage, mourner));
        longAtput((void *)((objStackPage + BaseHeaderSize) +
                           ((((usqInt)(iSqInt) << (shiftForWord()))))),
                  mourner);
      }
    }
    objStackPage =
        longAt((void *)((objStackPage + BaseHeaderSize) +
                        ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
  }
  /* end objStack:do: */
l1:;
}