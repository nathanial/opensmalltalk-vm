/* Extracted from interp.c:35892 (function fullGC). */

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

	GIV(needGCFlag) = 0;
	GIV(gcStartUsecs) = ioUTCMicrosecondsNow();
	GIV(statMarkCount) = 0;

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

	/* begin flushMethodCache */
	memset(GIV(methodCache), 0, MethodCacheSize * (sizeof(GIV(methodCache)[0])));

	/* this for primitiveExternalMethod */
	GIV(lastMethodCacheProbeWrite) = 0;

	/* begin flushAtCache */
	memset(GIV(atCache), 0, AtCacheTotalSize * (sizeof(GIV(atCache)[0])));
	assert(validObjStacks());
	assert(isEmptyObjStack(GIV(markStack)));
	assert(isEmptyObjStack(GIV(weaklingStack)));

	/* Mark objects /before/ scavenging, to empty the rememberedTable of unmarked roots. */
	markObjects(1);
	gcMarkEndUsecs = ioUTCMicrosecondsNow();

	/* begin forgetUnmarkedRememberedObjects */
	index = 0;
	while (index < GIV(rememberedSetSize)) {
		obj = GIV(rememberedSet)[index];
		if ((byteAt((void *)(obj + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) {
			index += 1;
		}
		else {
			setIsRememberedOfto(obj, 0);
			GIV(rememberedSetSize) -= 1;
			GIV(rememberedSet)[index] = (GIV(rememberedSet)[GIV(rememberedSetSize)]);
		}
	}
	assert(GIV(rememberedSetSize) >= 0);

	/* setGCMode: */
	doScavenge(MarkOnTenure);

	/* setGCMode: */

	/* Mid-way the leak check must be more lenient.  Unmarked classes will have been
	   expunged from the table, but unmarked instances will not yet have been reclaimed. */
	runLeakCheckerForexcludeUnmarkedObjsclassIndicesShouldBeValid(GCModeFull, 1, 1);
	compactionStartUsecs = ioUTCMicrosecondsNow();

	/* begin prepareForGlobalSweep */
	for (i = 0; i < GIV(numSegments); i += 1) {
		((GIV(segments)[i]).containsPinned = 0);
	}

	/* begin bridgeAt: */
	/* begin bridgeFor: */
	address = (((((&(GIV(segments)[GIV(numSegments) - 1])))->segSize)) + ((((&(GIV(segments)[GIV(numSegments) - 1])))->segStart))) - (2 * BaseHeaderSize);
	numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
	objOop = (numSlots == (numSlotsMask())
				? address + BaseHeaderSize
				: address);

	/* begin setIsMarkedOf:to: */
	assert(!(isFreeObject(objOop)));
	byteAtput((void *)(objOop + (markBitsByteOffset())),(byteAt((void *)(objOop + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
	compact();

	/* begin attemptToShrink */
	if ((GIV(totalFreeOldSpace) > GIV(shrinkThreshold))
	 && ((GIV(totalFreeOldSpace) > GIV(growHeadroom))
	 && (shrinkObjectMemory((((GIV(totalFreeOldSpace) - GIV(growHeadroom)) < GIV(growHeadroom)) ? GIV(growHeadroom) : (GIV(totalFreeOldSpace) - GIV(growHeadroom))))))) {
		GIV(statShrinkMemory) += 1;
	}

	/* begin setHeapSizeAtPreviousGC */
	GIV(heapSizeAtPreviousGC) = ((assert((totalBytesInSegments()) == GIV(totalHeapSizeIncludingBridges)),
GIV(totalHeapSizeIncludingBridges) - (GIV(numSegments) * (2 * BaseHeaderSize)))) - GIV(totalFreeOldSpace);

	/* begin resetAllocationAccountingAfterGC */
	GIV(oldSpaceUsePriorToScavenge) = ((assert((totalBytesInSegments()) == GIV(totalHeapSizeIncludingBridges)),
GIV(totalHeapSizeIncludingBridges) - (GIV(numSegments) * (2 * BaseHeaderSize)))) - GIV(totalFreeOldSpace);
	assert(validObjStacks());
	assert(isEmptyObjStack(GIV(markStack)));
	assert(isEmptyObjStack(GIV(weaklingStack)));
	assert(allObjectsUnmarked());
	runLeakCheckerFor(GCModeFull);

	/* begin postGCAction: */
	signalSemaphoreWithIndex(GIV(gcSemaphoreIndex));
	postGCUpdateDisplayBits();
	GIV(statGCEndUsecs) = ioUTCMicrosecondsNow();

	/* begin updateFullGCStats */
	GIV(statFullGCs) += 1;
	GIV(statFullGCUsecs) += GIV(statGCEndUsecs) - GIV(gcStartUsecs);
	GIV(statCompactionUsecs) += GIV(statGCEndUsecs) - compactionStartUsecs;
	if (GIV(gcSweepEndUsecs)) {
		GIV(statSweepUsecs) += GIV(gcSweepEndUsecs) - compactionStartUsecs;
	}
	GIV(statMarkUsecs) += gcMarkEndUsecs - GIV(gcStartUsecs);
	return (GIV(freeLists)[0]
			? bytesInBody(findLargestFreeChunk())
			: 0);
}