/* Extracted from interp.c:51192 (function checkProfileTickPostPrimitive). */

/*	Check if the profile timer has expired during a primitive and if so take
   a sample. If the primitive has failed sample the profileMethod as nil. */
/*	If continuing from primitive[Full]ClosureValueNoContextSwitch defer
   until the next check
 */

/* StackInterpreter>>#checkProfileTickPostPrimitive: */

static NeverInline void checkProfileTickPostPrimitive(sqInt aPrimitiveMethod) {
  sqInt objOop;

  if (deferProfileCheckForVNCS) {
    deferProfileCheckForVNCS = 0;
  } else {
    if ((nextProfileTick > 0) && ((ioHighResClock()) >= nextProfileTick)) {
      if (!profileProcess) {
        /* begin activeProcess */
        objOop = fetchPointerofObject(ValueIndex, fetchPointerofObject(SchedulerAssociation, specialObjectsOop));
        profileProcess = fetchPointerofObject(ActiveProcessIndex, objOop);
        if (primFailCode) {
          profileMethod = null;
          forceInterruptCheck();
        } else {
          profileMethod = aPrimitiveMethod;

          /* begin zeroNextProfileTick */
          nextProfileTick = 0;
          synchronousSignal(profileSemaphore);
        }
      }
    }
  }
}