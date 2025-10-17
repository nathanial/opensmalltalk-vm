/* Extracted from interp.c:46379 (function unfireQueuedEphemeronsForSnapshot).
 */

/*	Ephemerons in the mourn queue will have been fired (had their
        ephemeron-ness turned off),
        but the ephemeron queue is not saved in the snapshot. So the snapshotted
        ephemerons need to be unfired so they're still unqueued ephemerons in
   the loaded image. */

/* SpurMemoryManager>>#unfireQueuedEphemeronsForSnapshot */

static void unfireQueuedEphemeronsForSnapshot(void) {
  sqInt iSqInt;
  sqInt mourner;
  sqInt numOnThisPage;
  sqInt objStackPage;

  /* begin objStack:do: */
  if (mournQueue == nilObj) {
    goto l1;
  }
  eassert(isValidObjStack(mournQueue));
  objStackPage = mournQueue;
  while (objStackPage != 0) {
    numOnThisPage =
        fetchPointerofObject(ObjStackTopx, objStackPage);
    for (iSqInt = ((numOnThisPage + ObjStackFixedSlots) - 1);
         iSqInt >= ObjStackFixedSlots; iSqInt += -1) {
      mourner = fetchPointerofObject(iSqInt, objStackPage);
      if (((!(mourner & (tagMask())))) &&
          (((byteAt((void *)(mourner + (formatFieldByteOffset())))) &
            (formatMask())) == (nonIndexablePointerFormat()))) {
        /* begin setFormatOf:to: */
        assert((((ephemeronFormat()) >= 0) &&
                ((ephemeronFormat()) <= (formatMask()))));
        byteAtput((void *)(mourner + (formatFieldByteOffset())),
                  ((byteAt((void *)(mourner + (formatFieldByteOffset())))) &
                   (0xFF - (formatMask()))) +
                      (ephemeronFormat()));
      }
    }
    objStackPage =
        fetchPointerofObject(ObjStackNextx, objStackPage);
  }
  /* end objStack:do: */
l1:;
}