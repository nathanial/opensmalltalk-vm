/* Extracted from interp.c:66292 (function primitiveClone). */

static void
primitiveClone(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt newCopy;
    sqInt rcvr;
    char *sp;

	newCopy = 0;
	rcvr = longAt(GIV(stackPointer));
	if (((rcvr & (tagMask())) != 0)) {
		newCopy = rcvr;
	}
	else {
		if (((longAt((void *)(rcvr))) & (classIndexMask())) == ClassMethodContextCompactIndex) {
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
			newCopy = cloneContext(rcvr);
		}
		else {
			if ((GIV(argumentCount) == 0)
			 || (!((!((longAt((void *)(rcvr))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))) {
				newCopy = cloneObject(rcvr);
			}
		}
		if (!newCopy) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrNoMemory;
			return;
		}
	}

	/* begin methodReturnValue: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),newCopy);
	GIV(stackPointer) = sp;
}