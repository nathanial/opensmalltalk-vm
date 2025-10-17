/* Extracted from interp.c:51150 (function checkOkayStackZone). */

/*	Check that all objects in the stack zone are okay */

	/* StackInterpreter>>#checkOkayStackZone: */

static NoDbgRegParms sqInt
checkOkayStackZone(sqInt writeBack)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt ok;
    StackPage *thePage;

	if (writeBack) {
		/* begin externalWriteBackHeadFramePointers */
		assert((GIV(framePointer) - GIV(stackPointer)) < (LargeContextSlots * BytesPerOop));
		assert(GIV(stackPage) == (GIV(mostRecentlyUsedPage)));
		assert(!((isFree(GIV(stackPage)))));

		/* begin setHeadFP:andSP:inPage: */
		assert(GIV(stackPointer) < GIV(framePointer));
		assert((GIV(stackPointer) < ((GIV(stackPage)->baseAddress)))
		 && (GIV(stackPointer) > (((GIV(stackPage)->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
		assert((GIV(framePointer) < ((GIV(stackPage)->baseAddress)))
		 && (GIV(framePointer) > (((GIV(stackPage)->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
		(GIV(stackPage)->headFP = GIV(framePointer));
		(GIV(stackPage)->headSP = GIV(stackPointer));
		assert(pageListIsWellFormed());
	}
	ok = 1;
	for (i = 0; i < GIV(numStackPages); i += 1) {
		/* begin stackPageAt: */
		thePage = stackPageAtpages(i, GIV(pages));
		if (!(isFree(thePage))) {
			ok = ok && (checkOkayStackPage(thePage));
		}
	}
	return ok;
}