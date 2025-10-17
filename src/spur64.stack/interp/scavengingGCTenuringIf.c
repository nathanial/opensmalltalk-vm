/* Extracted from interp.c:45340 (function scavengingGCTenuringIf). */

/*	Run the scavenger. */

	/* SpurMemoryManager>>#scavengingGCTenuringIf: */

static NoDbgRegParms void
scavengingGCTenuringIf(sqInt tenuringCriterion)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt n;
    sqInt probe;

	assert(GIV(remapBufferCount) == 0);
	if (!(asserta(((((GIV(eden)).limit)) - GIV(freeStart)) > (interpreterAllocationReserveBytes())))) {
		/* begin tab */
		printChar('\t');
		n = (((GIV(eden)).limit)) - GIV(freeStart);

		/* begin printNum: */
		fprintf(GIV(transcript),
				"%" PRIdSQINT "",
				((sqInt)n));
		printChar(' ');
		n = interpreterAllocationReserveBytes();

		/* begin printNum: */
		fprintf(GIV(transcript),
				"%" PRIdSQINT "",
				((sqInt)n));
		printChar(' ');
		n = (interpreterAllocationReserveBytes()) - ((((GIV(eden)).limit)) - GIV(freeStart));

		/* begin printNum: */
		fprintf(GIV(transcript),
				"%" PRIdSQINT "",
				((sqInt)n));
		cr();
	}

	/* begin checkMemoryMap */
	assert(isYoungObject(GIV(newSpaceStart)));
	assert(!(isOldObject(GIV(newSpaceStart))));
	assert(isYoungObject(GIV(oldSpaceStart) - BytesPerWord));
	assert(!(isOldObject(GIV(oldSpaceStart) - BytesPerWord)));
	assert(!(isYoungObject(GIV(oldSpaceStart))));
	assert(isOldObject(GIV(oldSpaceStart)));
	assert(!(isYoungObject(GIV(endOfMemory))));
	assert(isOldObject(GIV(endOfMemory)));

	/* begin checkFreeSpace: */
	assert(bitsSetInFreeSpaceMaskForAllFreeLists());
	assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
	if (((checkForLeaks & (GCCheckFreeSpace | GCModeNewSpace)) == (GCCheckFreeSpace | GCModeNewSpace))) {
		runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
	}
	runLeakCheckerFor(GCModeNewSpace);

	/* begin preGCAction: */
	if (GIV(stackPage)) {
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

	/* begin flushMethodCacheFrom:to: */
	probe = 0;
	for (i = 1; i <= MethodCacheEntries; i += 1) {
		if (GIV(methodCache)[probe + MethodCacheSelector]) {
			if (((oopisGreaterThanOrEqualTo(GIV(methodCache)[probe + MethodCacheSelector], GIV(newSpaceStart)))
			 && (oopisLessThan(GIV(methodCache)[probe + MethodCacheSelector], GIV(oldSpaceStart))))
			 || ((oopisGreaterThanOrEqualTo(GIV(methodCache)[probe + MethodCacheMethod], GIV(newSpaceStart)))
			 && (oopisLessThan(GIV(methodCache)[probe + MethodCacheMethod], GIV(oldSpaceStart))))) {
				GIV(methodCache)[probe + MethodCacheSelector] = 0;
			}
		}
		probe += MethodCacheEntrySize;
	}

	/* begin flushAtCache */
	memset(GIV(atCache), 0, AtCacheTotalSize * (sizeof(GIV(atCache)[0])));

	/* would prefer this to be in mapInterpreterOops, but
	   compatibility with ObjectMemory dictates it goes here. */
	GIV(needGCFlag) = 0;
	GIV(gcStartUsecs) = ioUTCMicrosecondsNow();
	doScavenge(tenuringCriterion);
	GIV(statScavenges) += 1;
	GIV(statGCEndUsecs) = ioUTCMicrosecondsNow();
	GIV(statSGCDeltaUsecs) = GIV(statGCEndUsecs) - GIV(gcStartUsecs);
	GIV(statScavengeGCUsecs) += GIV(statSGCDeltaUsecs);
	GIV(statRootTableCount) = GIV(rememberedSetSize);

	/* begin logScavenge */
	if (GIV(scavengeLog)) {
		writeScavengeLog();
	}

	/* begin postGCAction: */
	signalSemaphoreWithIndex(GIV(gcSemaphoreIndex));
	postGCUpdateDisplayBits();
	runLeakCheckerFor(GCModeNewSpace);

	/* begin checkFreeSpace: */
	assert(bitsSetInFreeSpaceMaskForAllFreeLists());
	assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
	if (((checkForLeaks & (GCCheckFreeSpace | GCModeNewSpace)) == (GCCheckFreeSpace | GCModeNewSpace))) {
		runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
	}
}