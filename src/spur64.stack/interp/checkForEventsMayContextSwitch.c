/* Extracted from interp.c:50516 (function checkForEventsMayContextSwitch). */

/*	Check for possible interrupts and handle one if necessary.
        Answer if a context switch has occurred. */

/* StackInterpreter>>#checkForEventsMayContextSwitch: */

static sqInt
checkForEventsMayContextSwitch(sqInt mayContextSwitch) {
  sqInt heapSizePostGC;
  usqLong now;
  sqInt nowSqInt;
  sqInt objOop;
  sqInt sema;
  sqInt sufficientSpaceAfterGCRV;
  sqInt switched;

  statCheckForEvents += 1;

  /* restore the stackLimit if it has been smashed. */

  /* begin restoreStackLimit */
  (stackPage->stackLimit = (stackPage->realStackLimit));
  stackLimit = (stackPage->stackLimit);

  /* begin externalWriteBackHeadFramePointers */
  assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
  assert(stackPage == (mostRecentlyUsedPage));
  assert(!((isFree(stackPage))));

  /* begin setHeadFP:andSP:inPage: */
  assert(stackPointer < framePointer);
  assert((stackPointer < ((stackPage->baseAddress))) &&
         (stackPointer >
          (((stackPage->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
  assert((framePointer < ((stackPage->baseAddress))) &&
         (framePointer > (((stackPage->realStackLimit)) -
                          ((LargeContextSlots * BytesPerOop) / 2))));
  (stackPage->headFP = framePointer);
  (stackPage->headSP = stackPointer);
  assert(pageListIsWellFormed());
  assert(stackPage == (mostRecentlyUsedPage));

  /* Allow the platform to do anything it needs to do synchronously. */
  ioSynchronousCheckForEvents();
  if (needGCFlag) {
    /* begin sufficientSpaceAfterGC: */
    scavengingGCTenuringIf(TenureByAge);
    heapSizePostGC =
        ((/* begin totalOldSpaceCapacity */
          assert((totalBytesInSegments()) == totalHeapSizeIncludingBridges),
          totalHeapSizeIncludingBridges -
              (numSegments * (2 * BaseHeaderSize)))) -
        totalFreeOldSpace;
    if (((((double)(heapSizePostGC - heapSizeAtPreviousGC))) /
         heapSizeAtPreviousGC) >= heapGrowthToSizeGCRatio) {
      fullGC();
    } else {
      if (totalFreeOldSpace > (shrinkThreshold * 2)) {
        /* begin attemptToShrink */
        if ((totalFreeOldSpace > shrinkThreshold) &&
            ((totalFreeOldSpace > growHeadroom) &&
             (shrinkObjectMemory(
                 (((totalFreeOldSpace - growHeadroom) < growHeadroom)
                      ? growHeadroom
                      : (totalFreeOldSpace - growHeadroom)))))) {
          statShrinkMemory += 1;
        }
        sufficientSpaceAfterGCRV = 1;
        goto l1;
      }
    }

    /* Also attempt to shrink if there is plenty of free space and no need to GC
     */
    while ((totalFreeOldSpace < growHeadroom) && ((growOldSpaceByAtLeast(0)))) {
      if (totalFreeOldSpace >= growHeadroom) {
        sufficientSpaceAfterGCRV = 1;
        goto l1;
      }
    }

    /* Answer false if space is low */
    sufficientSpaceAfterGCRV = lowSpaceThreshold <= totalFreeOldSpace;
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
  if ((nextProfileTick <= 0) || ((ioHighResClock()) < nextProfileTick)) {
    switched = 0;
    goto l2;
  }

  /* Take a sample (if not already done so) for the profiler. */
  if (!profileProcess) {
    /* begin activeProcess */
    objOop =
        longAt((void *)(((longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                                          ((((usqInt)(SchedulerAssociation)
                                             << (shiftForWord()))))))) +
                         BaseHeaderSize) +
                        ((((usqInt)(ValueIndex) << (shiftForWord()))))));
    profileProcess = longAt(
        (void *)((objOop + BaseHeaderSize) +
                 ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));
    profileMethod = null;
  }

  /* Zero nextProfileTick because signalling the semaphore is when the VM
   * effectively delivers the sample. */

  /* begin zeroNextProfileTick */
  nextProfileTick = 0;
  switched = synchronousSignal(profileSemaphore);
  /* end checkProfileTickIfSwitched */
l2:
#if LRPCheck
  if (checkDeliveryOfLongRunningPrimitiveSignal()) {
    switched = 1;
  }
#endif

  if (signalLowSpace) {
    /* signalLowSpace: */
    signalLowSpace = 0;
    sema = longAt(
        (void *)((specialObjectsOop + BaseHeaderSize) +
                 ((((usqInt)(TheLowSpaceSemaphore) << (shiftForWord()))))));
    if ((sema != nilObj) && (synchronousSignal(sema))) {
      switched = 1;
    }
  }
  nowSqInt = (now = ioUTCMicroseconds());

  /* begin checkInvokeIOProcessEvents: */
  if (nowSqInt >= nextPollUsecs) {
    statIOProcessEvents += 1;
    ioProcessEvents();

    /* msecs to wait before next call to ioProcessEvents.  Note that strictly
       speaking we might need to update 'now' at this point since
       ioProcessEvents could take a very long time on some platforms */
    nextPollUsecs = nowSqInt + 20000;
  }
  if (interruptPending) {
    interruptPending = 0;

    /* reset interrupt flag */
    sema = longAt(
        (void *)((specialObjectsOop + BaseHeaderSize) +
                 ((((usqInt)(TheInterruptSemaphore) << (shiftForWord()))))));
    if ((sema != nilObj) && (synchronousSignal(sema))) {
      switched = 1;
    }
  }
  if (nextWakeupUsecs) {
    if (now >= nextWakeupUsecs) {
      nextWakeupUsecs = 0;

      /* set timer interrupt to 0 for 'no timer' */
      sema = longAt(
          (void *)((specialObjectsOop + BaseHeaderSize) +
                   ((((usqInt)(TheTimerSemaphore) << (shiftForWord()))))));
      if ((sema != nilObj) && (synchronousSignal(sema))) {
        switched = 1;
      }
    }
  }

  /* signal any pending finalizations */
  if (pendingFinalizationSignals > 0) {
    pendingFinalizationSignals = 0;
    sema = longAt(
        (void *)((specialObjectsOop + BaseHeaderSize) +
                 ((((usqInt)(TheFinalizationSemaphore) << (shiftForWord()))))));
    if ((sema != nilObj) && (synchronousSignal(sema))) {
      switched = 1;
    }
  }

  /* signal all semaphores in semaphoresToSignal */
  if (signalExternalSemaphores()) {
    switched = 1;
  }
  return switched;
}