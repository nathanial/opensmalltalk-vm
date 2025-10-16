/* Extracted from interp.c:14476 (function primitiveControlVMProfiling). */

/*	Primitive. Start or stop the VM profiler. The first argument is a
   boolean to switch profiling on or off. The second argument is an integer or
   nil. If an integer it determines the maximum number of samples in the VM's
        sample buffer. Answer the current number of samples in the buffer. */

/* InterpreterPrimitives>>#primitiveControlVMProfiling */

static void primitiveControlVMProfiling(void) {
  sqInt bufferSize;
  sqInt numSamples;
  sqInt onOffBar;
  char *sp;

  if (argumentCount != 2) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }
  if (((onOffBar = longAt(stackPointer + (1 * BytesPerWord)))) == trueObj) {
    onOffBar = 1;
  } else {
    if (onOffBar == falseObj) {
      onOffBar = 0;
    } else {
      /* begin primitiveFail */
      if (!primFailCode) {
        primFailCode = 1;
      }
      return;
    }
  }
  if (((bufferSize = longAt(stackPointer))) == nilObj) {
    bufferSize = 0;
  } else {
    if (!(((((bufferSize) & 7) == 1)) &&
          (((bufferSize = (bufferSize >> 3))) > 0))) {
      /* begin primitiveFail */
      if (!primFailCode) {
        primFailCode = 1;
      }
      return;
    }
  }
  numSamples = ioControlNewProfile(onOffBar, bufferSize);

  /* begin pop:thenPushInteger: */
  longAtput((sp = stackPointer + (2 * BytesPerWord)),
            (((usqInt)numSamples << 3) | 1));
  stackPointer = sp;
}