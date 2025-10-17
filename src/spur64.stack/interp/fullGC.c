/* Extracted from interp.c:35892 (function fullGC). */

/*	Perform a full eager compacting GC. Answer the size of the largest free
	chunk. 
 */
/*	for profiling */

	/* SpurMemoryManager>>#fullGC */

NeverInline usqLong
fullGC(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    usqLong compactionStartUsecs;
    usqLong gcMarkEndUsecs;
    sqInt i;
    sqInt index;
    usqInt numSlots;
    sqInt obj;
    sqInt objOop;

	needGCFlag = 0;
	gcStartUsecs = ioUTCMicrosecondsNow();
	statMarkCount = 0;

	/* begin preGCAction: */
	if (stackPage) {
		/* begin externalWriteBackHeadFramePointers */
		assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
		assert(stackPage == (mostRecentlyUsedPage));
		assert(!((isFree(stackPage))));

		/* begin setHeadFP:andSP:inPage: */
		assert(stackPointer < framePointer);
		assert((stackPointer < ((stackPage->baseAddress)))
		 && (stackPointer > (((stackPage->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
		assert((framePointer < ((stackPage->baseAddress)))
		 && (framePointer > (((stackPage->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
		(stackPage->headFP = framePointer);
		(stackPage->headSP = stackPointer);
		assert(pageListIsWellFormed());
	}

	/* begin flushMethodCache */
	memset(methodCache, 0, MethodCacheSize * (sizeof(methodCache[0])));

	/* this for primitiveExternalMethod */
	lastMethodCacheProbeWrite = 0;

	/* begin flushAtCache */
	memset(atCache, 0, AtCacheTotalSize * (sizeof(atCache[0])));
	assert(validObjStacks());
	assert(isEmptyObjStack(markStack));
	assert(isEmptyObjStack(weaklingStack));

	/* Mark objects /before/ scavenging, to empty the rememberedTable of unmarked roots. */
	markObjects(1);
	gcMarkEndUsecs = ioUTCMicrosecondsNow();

	/* begin forgetUnmarkedRememberedObjects */
	index = 0;
	while (index < rememberedSetSize) {
		obj = rememberedSet[index];
		if ((byteAt((void *)(obj + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) {
			index += 1;
		}
		else {
			setIsRememberedOfto(obj, 0);
			rememberedSetSize -= 1;
			rememberedSet[index] = (rememberedSet[rememberedSetSize]);
		}
	}
	assert(rememberedSetSize >= 0);

	/* setGCMode: */
	doScavenge(MarkOnTenure);

	/* setGCMode: */

	/* Mid-way the leak check must be more lenient.  Unmarked classes will have been
	   expunged from the table, but unmarked instances will not yet have been reclaimed. */
	runLeakCheckerForexcludeUnmarkedObjsclassIndicesShouldBeValid(GCModeFull, 1, 1);
	compactionStartUsecs = ioUTCMicrosecondsNow();

	/* begin prepareForGlobalSweep */
	for (i = 0; i < numSegments; i += 1) {
		((segments[i]).containsPinned = 0);
	}

	/* begin bridgeAt: */
	/* begin bridgeFor: */
	address = (((((&(segments[numSegments - 1])))->segSize)) + ((((&(segments[numSegments - 1])))->segStart))) - (2 * BaseHeaderSize);
	numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
	objOop = (numSlots == (numSlotsMask())
				? address + BaseHeaderSize
				: address);

	/* begin setIsMarkedOf:to: */
	assert(!(isFreeObject(objOop)));
	byteAtput((void *)(objOop + (markBitsByteOffset())),(byteAt((void *)(objOop + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
	compact();

	/* begin attemptToShrink */
	if ((totalFreeOldSpace > shrinkThreshold)
	 && ((totalFreeOldSpace > growHeadroom)
	 && (shrinkObjectMemory((((totalFreeOldSpace - growHeadroom) < growHeadroom) ? growHeadroom : (totalFreeOldSpace - growHeadroom)))))) {
		statShrinkMemory += 1;
	}

	/* begin setHeapSizeAtPreviousGC */
	heapSizeAtPreviousGC = ((assert((totalBytesInSegments()) == totalHeapSizeIncludingBridges),
totalHeapSizeIncludingBridges - (numSegments * (2 * BaseHeaderSize)))) - totalFreeOldSpace;

	/* begin resetAllocationAccountingAfterGC */
	oldSpaceUsePriorToScavenge = ((assert((totalBytesInSegments()) == totalHeapSizeIncludingBridges),
totalHeapSizeIncludingBridges - (numSegments * (2 * BaseHeaderSize)))) - totalFreeOldSpace;
	assert(validObjStacks());
	assert(isEmptyObjStack(markStack));
	assert(isEmptyObjStack(weaklingStack));
	assert(allObjectsUnmarked());
	runLeakCheckerFor(GCModeFull);

	/* begin postGCAction: */
	signalSemaphoreWithIndex(gcSemaphoreIndex);
	postGCUpdateDisplayBits();
	statGCEndUsecs = ioUTCMicrosecondsNow();

	/* begin updateFullGCStats */
	statFullGCs += 1;
	statFullGCUsecs += statGCEndUsecs - gcStartUsecs;
	statCompactionUsecs += statGCEndUsecs - compactionStartUsecs;
	if (gcSweepEndUsecs) {
		statSweepUsecs += gcSweepEndUsecs - compactionStartUsecs;
	}
	statMarkUsecs += gcMarkEndUsecs - gcStartUsecs;
	return (freeLists[0]
			? bytesInBody(findLargestFreeChunk())
			: 0);
}