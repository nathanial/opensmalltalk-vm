/* Extracted from interp.c:65760 (function wakeHighestPriority). */

static sqInt
wakeHighestPriority(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ctxt;
    usqInt numSlots;
    sqInt objOop;
    sqInt p;
    sqInt proc;
    sqInt processList;
    sqInt schedLists;

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
	objOop = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));

	/* begin fetchPointer:ofObject: */
	schedLists = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ProcessListsIndex) << (shiftForWord()))))));
	p = (GIV(highestRunnableProcessPriority)
				? GIV(highestRunnableProcessPriority)
				: (/* begin numSlotsOf: */
					assert((classIndexOf(schedLists)) > (isForwardedObjectClassIndexPun())),
				(((numSlots = byteAt((void *)(schedLists + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
							? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(schedLists - BaseHeaderSize)))) << 8)))))) >> 8
							: numSlots)));
	while (((p -= 1)) >= 0) {
		processList = longAt((void *)((schedLists + BaseHeaderSize) + ((((usqInt)(p) << (shiftForWord()))))));
		while (1) {
			/* begin isEmptyList: */
			assert(!(isForwarded(processList)));
			if ((longAt((void *)((processList + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))))) == GIV(nilObj)) break;
			proc = removeFirstLinkOfList(processList);
			ctxt = longAt((void *)((proc + BaseHeaderSize) + ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))));
			if (isLiveContext(ctxt)) {
				GIV(highestRunnableProcessPriority) = p + 1;
				return proc;
			}
			if (/* isOopForwarded: */
				((!(ctxt & (tagMask()))))
			 && ((!((longAt((void *)(ctxt))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
				ctxt = fixFollowedFieldofObjectwithInitialValue(SuspendedContextIndex, proc, ctxt);
			}
			if (isLiveContext(ctxt)) {
				GIV(highestRunnableProcessPriority) = p + 1;
				return proc;
			}

			/* This is uncommon, so we can deal with forwarders here instead of assuming there isn't. */
			warning("evicted zombie process from run queue");
		}
	}
	return null;
}