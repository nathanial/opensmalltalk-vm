/* Extracted from interp.c:44951 (function refireQueuedEphemeronsPostSnapshot). */

/*	Ephemerons in the mourn queue will have been fired (had their
	ephemeron-ness turned off),
	but the ephemeron queue is not saved in the snapshot. So the snapshotted
	ephemerons need to be unfired so they're still unqueued ephemerons in the
	loaded image.
	This must be undone post snapshot. */

	/* SpurMemoryManager>>#refireQueuedEphemeronsPostSnapshot */

static void
refireQueuedEphemeronsPostSnapshot(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
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
		numOnThisPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
		for (iSqInt = ((numOnThisPage + ObjStackFixedSlots) - 1); iSqInt >= ObjStackFixedSlots; iSqInt += -1) {
			mourner = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(iSqInt) << (shiftForWord()))))));
			if (((!(mourner & (tagMask()))))
			 && (((byteAt((void *)(mourner + (formatFieldByteOffset())))) & (formatMask())) == (ephemeronFormat()))) {
				/* begin setFormatOf:to: */
				assert((((nonIndexablePointerFormat()) >= 0) && ((nonIndexablePointerFormat()) <= (formatMask()))));
				byteAtput((void *)(mourner + (formatFieldByteOffset())),((byteAt((void *)(mourner + (formatFieldByteOffset())))) & (0xFF - (formatMask()))) + (nonIndexablePointerFormat()));
			}
		}
		objStackPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
	}
	/* end objStack:do: */
l1:;
}