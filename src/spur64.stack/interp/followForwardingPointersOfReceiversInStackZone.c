/* Extracted from interp.c:53740 (function followForwardingPointersOfReceiversInStackZone). */

static void
followForwardingPointersOfReceiversInStackZone(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt aValue;
    char *callerFP;
    sqInt delta;
    sqInt i;
    sqInt newOop;
    sqInt offset;
    sqInt oop;
    sqInt referent;
    char *theFP;
    usqInt theIPPtr;
    StackPage *thePage;

	callerFP = ((char *) 0);
	if (!GIV(stackPage)) {
		assert((!(GIV(mostRecentlyUsedPage)))
		 || (isFree(GIV(mostRecentlyUsedPage))));
		return;
	}

	/* the system must be snapshotting; nothing to do... */

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
	for (i = 0; i < GIV(numStackPages); i += 1) {
		/* begin stackPageAt: */
		thePage = stackPageAtpages(i, GIV(pages));
		if ((thePage->baseFP)) {
			assert(ifCurrentStackPageHasValidHeadPointers(thePage));
			theFP = (thePage->headFP);

			/* Skip the instruction pointer on top of stack of inactive pages. */
			theIPPtr = (!(thePage == GIV(stackPage))
						? ((usqInt)((thePage->headSP)))
						: 0);
			while (1) {
				assert(addressIsInPage(thePage, theFP));
				assert((!theIPPtr)
				 || (addressIsInPage(thePage, ((void *)theIPPtr))));
				oop = longAt(theFP + FoxReceiver);
				if (/* isOopForwarded: */
					((!(oop & (tagMask()))))
				 && ((!((longAt((void *)(oop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					longAtput(theFP + FoxReceiver,followForwarded(oop));
				}
				if (((byteAt((theFP + FoxFrameFlags) + 2)) != 0)
				 && ((!((longAt((void *)(longAt(theFP + FoxThisContext)))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					longAtput(theFP + FoxThisContext,followForwarded(longAt(theFP + FoxThisContext)));
				}
				oop = longAt(theFP + FoxMethod);
				if ((!((longAt((void *)(oop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
					/* begin followForwarded: */
					assert(isUnambiguouslyForwarder(oop));
					newOop = longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
					while (/* isOopForwarded: */
						((!(newOop & (tagMask()))))
					 && ((!((longAt((void *)(newOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
						newOop = longAt((void *)((newOop + BaseHeaderSize) + (0U << (shiftForWord()))));
					}
					if (theIPPtr) {
						assert((longAt((void *)(theIPPtr))) > (frameMethod(theFP)));
						delta = newOop - oop;
						longAtput((void *)(theIPPtr),(longAt((void *)(theIPPtr))) + delta);
					}
					longAtput(theFP + FoxMethod,(oop = newOop));
				}
				offset = (FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1))) << (shiftForWord()))));
				oop = longAt(theFP + offset);
				if (/* isOopForwarded: */
					((!(oop & (tagMask()))))
				 && ((!((longAt((void *)(oop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					longAtput(theFP + offset,followForwarded(oop));
				}
				if (!(((callerFP = ((char *)(longAt(theFP + FoxSavedFP))))) != 0)) break;
				theIPPtr = ((usqInt)(theFP + FoxCallerSavedIP));
				theFP = callerFP;
			}

			/* And finally follow the caller context. */
			assert(theFP == ((thePage->baseFP)));

			/* begin frameCallerContext: */
			assert(isBaseFrame(theFP));
			oop = longAt(theFP + FoxCallerContext);
			if ((!((longAt((void *)(oop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
				/* begin followForwarded: */
				assert(isUnambiguouslyForwarder(oop));
				referent = longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
				while (/* isOopForwarded: */
					((!(referent & (tagMask()))))
				 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
				}
				aValue = referent;

				/* begin frameCallerContext:put: */
				assert(isBaseFrame(theFP));
				assert((aValue == (nilObject()))
				 || (isContext(aValue)));
				longAtput(theFP + FoxCallerContext,aValue);
			}
		}
	}
}