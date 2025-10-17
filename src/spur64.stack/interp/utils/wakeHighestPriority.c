/* Extracted from interp.c:65760 (function wakeHighestPriority). */

/*	Return the highest priority process that is ready to run.
        To save time looking at many empty lists before finding a
        runnable process the VM maintains a variable holding the
        highest priority runnable process. If this variable is 0 then the
        VM does not know the highest priority and must search all lists.
        Answer nil if no process can be found. */

/* StackInterpreter>>#wakeHighestPriority */

static sqInt wakeHighestPriority(void) {
  sqInt ctxt;
  usqInt numSlots;
  sqInt objOop;
  sqInt p;
  sqInt proc;
  sqInt processList;
  sqInt schedLists;

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
  objOop =
      longAt((void *)(((longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                                        ((((usqInt)(SchedulerAssociation)
                                           << (shiftForWord()))))))) +
                       BaseHeaderSize) +
                      ((((usqInt)(ValueIndex) << (shiftForWord()))))));

  /* begin fetchPointer:ofObject: */
  schedLists =
      longAt((void *)((objOop + BaseHeaderSize) +
                      ((((usqInt)(ProcessListsIndex) << (shiftForWord()))))));
  p = (highestRunnableProcessPriority
           ? highestRunnableProcessPriority
           : (/* begin numSlotsOf: */
              assert((classIndexOf(schedLists)) >
                     (isForwardedObjectClassIndexPun())),
              (((numSlots = byteAt(
                     (void *)(schedLists + (numSlotsFieldByteOffset()))))) ==
                       (numSlotsMask())
                   ? ((((usqInt)((
                         (sqInt)((usqInt)((longAt(
                                     (void *)(schedLists - BaseHeaderSize))))
                                 << 8)))))) >>
                         8
                   : numSlots)));
  while (((p -= 1)) >= 0) {
    processList = longAt((void *)((schedLists + BaseHeaderSize) +
                                  ((((usqInt)(p) << (shiftForWord()))))));
    while (1) {
      /* begin isEmptyList: */
      assert(!(isForwarded(processList)));
      if ((longAt((void *)((processList + BaseHeaderSize) +
                           ((((usqInt)(FirstLinkIndex)
                              << (shiftForWord()))))))) == nilObj)
        break;
      proc = removeFirstLinkOfList(processList);
      ctxt = longAt(
          (void *)((proc + BaseHeaderSize) +
                   ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))));
      if (isLiveContext(ctxt)) {
        highestRunnableProcessPriority = p + 1;
        return proc;
      }
      if (isOopForwarded(ctxt)) {
        ctxt = fixFollowedFieldofObjectwithInitialValue(SuspendedContextIndex,
                                                        proc, ctxt);
      }
      if (isLiveContext(ctxt)) {
        highestRunnableProcessPriority = p + 1;
        return proc;
      }

      /* This is uncommon, so we can deal with forwarders here instead of
       * assuming there isn't. */
      warning("evicted zombie process from run queue");
    }
  }
  return null;
}