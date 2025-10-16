/* Extracted from interp.c:50516 (function checkForEventsMayContextSwitch). */

static NoDbgRegParms sqInt
checkForEventsMayContextSwitch(sqInt mayContextSwitch)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt heapSizePostGC;
    usqLong now;
    sqInt nowSqInt;
    sqInt objOop;
    sqInt sema;
    sqInt sufficientSpaceAfterGCRV;
    sqInt switched;

	GIV(statCheckForEvents) += 1;

	/* restore the stackLimit if it has been smashed. */

	/* begin restoreStackLimit */
	(GIV(stackPage)->stackLimit = (GIV(stackPage)->realStackLimit));
	GIV(stackLimit) = (GIV(stackPage)->stackLimit);

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
	assert(GIV(stackPage) == (GIV(mostRecentlyUsedPage)));

	/* Allow the platform to do anything it needs to do synchronously. */
	ioSynchronousCheckForEvents();
	if (GIV(needGCFlag)) {
		/* begin sufficientSpaceAfterGC: */
		scavengingGCTenuringIf(TenureByAge);
		heapSizePostGC = ((/* begin totalOldSpaceCapacity */
	assert((totalBytesInSegments()) == GIV(totalHeapSizeIncludingBridges)),
GIV(totalHeapSizeIncludingBridges) - (GIV(numSegments) * (2 * BaseHeaderSize)))) - GIV(totalFreeOldSpace);
		if (((((double) (heapSizePostGC - GIV(heapSizeAtPreviousGC)) )) / GIV(heapSizeAtPreviousGC)) >= GIV(heapGrowthToSizeGCRatio)) {
			fullGC();
		}
		else {
			if (GIV(totalFreeOldSpace) > (GIV(shrinkThreshold) * 2)) {
				/* begin attemptToShrink */
				if ((GIV(totalFreeOldSpace) > GIV(shrinkThreshold))
				 && ((GIV(totalFreeOldSpace) > GIV(growHeadroom))
				 && (shrinkObjectMemory((((GIV(totalFreeOldSpace) - GIV(growHeadroom)) < GIV(growHeadroom)) ? GIV(growHeadroom) : (GIV(totalFreeOldSpace) - GIV(growHeadroom))))))) {
					GIV(statShrinkMemory) += 1;
				}
				sufficientSpaceAfterGCRV = 1;
				goto l1;
			}
		}

		/* Also attempt to shrink if there is plenty of free space and no need to GC */
		while ((GIV(totalFreeOldSpace) < GIV(growHeadroom))
		 && ((growOldSpaceByAtLeast(0)))) {
			if (GIV(totalFreeOldSpace) >= GIV(growHeadroom)) {
				sufficientSpaceAfterGCRV = 1;
				goto l1;
			}
		}

		/* Answer false if space is low */
		sufficientSpaceAfterGCRV = GIV(lowSpaceThreshold) <= GIV(totalFreeOldSpace);
		/* end sufficientSpaceAfterGC: */
l1:
		if (!sufficientSpaceAfterGCRV) {
			setSignalLowSpaceFlagAndSaveProcess();
		}
	}

	/* sufficientSpaceAfterGC: runs the incremental GC and
	   then, if not enough space is available, the fullGC. */
	if (!mayContextSwitch) {
		return 0;
	}

	/* begin checkProfileTickIfSwitched */
	if ((GIV(nextProfileTick) <= 0)
	 || ((ioHighResClock()) < GIV(nextProfileTick))) {
		switched = 0;
		goto l2;
	}

	/* Take a sample (if not already done so) for the profiler. */
	if (!GIV(profileProcess)) {
		/* begin activeProcess */
		objOop = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
		GIV(profileProcess) = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));
		GIV(profileMethod) = null;
	}

	/* Zero nextProfileTick because signalling the semaphore is when the VM effectively delivers the sample. */

	/* begin zeroNextProfileTick */
	GIV(nextProfileTick) = 0;
	switched = synchronousSignal(GIV(profileSemaphore));
	/* end checkProfileTickIfSwitched */
l2:
#  if LRPCheck
	if (checkDeliveryOfLongRunningPrimitiveSignal()) {
		switched = 1;
	}
#  endif

	if (GIV(signalLowSpace)) {
		/* signalLowSpace: */
		GIV(signalLowSpace) = 0;
		sema = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheLowSpaceSemaphore) << (shiftForWord()))))));
		if ((sema != GIV(nilObj))
		 && (synchronousSignal(sema))) {
			switched = 1;
		}
	}
	nowSqInt = (now = ioUTCMicroseconds());

	/* begin checkInvokeIOProcessEvents: */
	if (nowSqInt >= GIV(nextPollUsecs)) {
		GIV(statIOProcessEvents) += 1;
		ioProcessEvents();

		/* msecs to wait before next call to ioProcessEvents.  Note that strictly
		   speaking we might need to update 'now' at this point since
		   ioProcessEvents could take a very long time on some platforms */
		GIV(nextPollUsecs) = nowSqInt + 20000;
	}
	if (GIV(interruptPending)) {
		GIV(interruptPending) = 0;

		/* reset interrupt flag */
		sema = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheInterruptSemaphore) << (shiftForWord()))))));
		if ((sema != GIV(nilObj))
		 && (synchronousSignal(sema))) {
			switched = 1;
		}
	}
	if (GIV(nextWakeupUsecs)) {
		if (now >= GIV(nextWakeupUsecs)) {
			GIV(nextWakeupUsecs) = 0;

			/* set timer interrupt to 0 for 'no timer' */
			sema = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheTimerSemaphore) << (shiftForWord()))))));
			if ((sema != GIV(nilObj))
			 && (synchronousSignal(sema))) {
				switched = 1;
			}
		}
	}

	/* signal any pending finalizations */
	if (GIV(pendingFinalizationSignals) > 0) {
		GIV(pendingFinalizationSignals) = 0;
		sema = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheFinalizationSemaphore) << (shiftForWord()))))));
		if ((sema != GIV(nilObj))
		 && (synchronousSignal(sema))) {
			switched = 1;
		}
	}

	/* signal all semaphores in semaphoresToSignal */
	if (signalExternalSemaphores()) {
		switched = 1;
	}
	return switched;
}