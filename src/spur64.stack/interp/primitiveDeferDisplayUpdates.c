/* Extracted from interp.c:67350 (function primitiveDeferDisplayUpdates). */

/* VMInvestigations */

/*	Set or clear the flag that controls whether modifications of
        the Display object are propagated to the underlying
        platform's screen. */

/* StackInterpreterPrimitives>>#primitiveDeferDisplayUpdates */

static void primitiveDeferDisplayUpdates(void) {
  sqInt flag;

  if (cannotDeferDisplayUpdates) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }
  flag = longAt(stackPointer);
  if (flag == trueObj) {
    deferDisplayUpdates = 1;
  } else {
    if (flag == falseObj) {
      deferDisplayUpdates = 0;
    } else {
      /* begin primitiveFail */
      if (!primFailCode) {
        primFailCode = 1;
      }
      return;
    }
  }

  /* begin pop: */
  stackPointer += 1 * BytesPerWord;
}