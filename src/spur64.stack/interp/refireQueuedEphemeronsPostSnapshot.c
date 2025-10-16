/* Extracted from interp.c:44951 (function refireQueuedEphemeronsPostSnapshot). */

static void
refireQueuedEphemeronsPostSnapshot(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt iSqInt;
    sqInt mourner;
    sqInt numOnThisPage;
    sqInt objStackPage;

	/* begin objStack:do: */
	if (GIV(mournQueue) == GIV(nilObj)) {
		goto l1;
	}
	eassert(isValidObjStack(GIV(mournQueue)));
	objStackPage = GIV(mournQueue);
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