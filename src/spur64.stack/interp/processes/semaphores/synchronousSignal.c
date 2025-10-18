/* Extracted from interp.c:65033 (function synchronousSignal). */

/*	Signal the given semaphore from within the interpreter.
        Answer if the current process was preempted. */

/* StackInterpreter>>#synchronousSignal: */

static sqInt synchronousSignal(sqInt aSemaphore) {
  sqInt ctxt;
  sqInt excessSignals;
  sqInt integerValue;
  sqInt proc;
  sqInt referent;

  assert((addressCouldBeObj(aSemaphore)) &&
         ((!(isForwarded(aSemaphore))) &&
          ((numSlotsOf(aSemaphore)) > ExcessSignalsIndex)));

  /* begin isEmptyList: */
  assert(!(isForwarded(aSemaphore)));
  if ((fetchPointerofObject(FirstLinkIndex, aSemaphore)) == nilObj) {
    excessSignals = fetchIntegerofObject(ExcessSignalsIndex, aSemaphore);
    integerValue = excessSignals + 1;

    /* begin storeInteger:ofObject:withValue: */
    if ((((((usqInt)(integerValue)) >> 60) + 1) & 15) <= 1) {
      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(aSemaphore)) && (!(isForwarded(aSemaphore))));
      assert(validStorePointerUncheckedArgs(ExcessSignalsIndex, aSemaphore,
                                            (((usqInt)integerValue << 3) | 1)));
      longAtput(
          (void *)((aSemaphore + BaseHeaderSize) +
                   ((((usqInt)(ExcessSignalsIndex) << (shiftForWord()))))),
          (((usqInt)integerValue << 3) | 1));
    } else {
      /* begin primitiveFail */
      if (!primFailCode) {
        primFailCode = 1;
      }
    }
    return 0;
  }

  /* no process is waiting on this semaphore */

  /* begin ensureSemaphoreUnforwardedThroughContext: */
  proc = fetchPointerofObject(FirstLinkIndex, aSemaphore);
  if ((!((longAt((void *)(proc))) &
         ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
    followForwardedObjectFieldstoDepth(aSemaphore, 1);
    proc = fetchPointerofObject(FirstLinkIndex, aSemaphore);
  }
  assert(!((isForwarded(proc))));
  ctxt = fetchPointerofObject(SuspendedContextIndex, proc);
  if ((!((longAt((void *)(ctxt))) &
         ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
    ctxt = followForwarded(ctxt);

    storePointerofObjectwithValue(SuspendedContextIndex, proc, ctxt);
  }
  return resumepreemptedYieldingIffrom(removeFirstLinkOfList(aSemaphore),
                                       preemptionYields, CSSynchronousSignal);
}