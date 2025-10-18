/* Extracted from interp.c:63049 (function setSignalLowSpaceFlagAndSaveProcess).
 */

/*	The low space semaphore is about to be signaled. Set the signalLowSpace
        flag, and force an interrupt check. Save the currently active process in
        the special
        objects array so that the low space handler will be able to determine
   the process that first triggered a low space condition. The image's low space
        handler is expected
        to nil out the special objects array slot when it handles the low space
        condition.  */

/* StackInterpreter>>#setSignalLowSpaceFlagAndSaveProcess */

static void setSignalLowSpaceFlagAndSaveProcess(void) {
  sqInt activeProc;
  sqInt lastSavedProcess;
  sqInt objOop;

  /* signalLowSpace: */
  signalLowSpace = 1;

  /* begin lowSpaceThreshold: */
  lowSpaceThreshold = 0;

  /* N.B. The threshold > 0 guard eliminates a warning when
     self lowSpaceThreshold: 0
     is inlined into setSignalLowSpaceFlagAndSaveProcess */
  assert(totalFreeOldSpace >= lowSpaceThreshold);
  lastSavedProcess =
      fetchPointerofObject(ProcessSignalingLowSpace, specialObjectsOop);
  if (lastSavedProcess == nilObj) {
    /* begin activeProcess */
    objOop = fetchPointerofObject(
        ValueIndex,
        fetchPointerofObject(SchedulerAssociation, specialObjectsOop));
    activeProc = fetchPointerofObject(ActiveProcessIndex, objOop);

    /* begin splObj:put: */
    storePointerofObjectwithValue(ProcessSignalingLowSpace, specialObjectsOop, activeProc);
  }
  forceInterruptCheck();
}