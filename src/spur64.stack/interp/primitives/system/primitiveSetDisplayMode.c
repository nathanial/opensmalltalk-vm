/* Extracted from interp.c:21928 (function primitiveSetDisplayMode). */

/*	Ask the GUI to set the requested display mode.
        See DisplayScreen class depth:width:height:fullscreen: */

/* InterpreterPrimitives>>#primitiveSetDisplayMode */

static void primitiveSetDisplayMode(void) {
  sqInt d;
  sqInt fsFlag;
  sqInt h;
  sqInt integerPointer;
  sqInt okay;
  char *sp;
  sqInt w;

  /* begin booleanValueOf: */
  if ((longAt(stackPointer)) == trueObj) {
    fsFlag = 1;
    goto l1;
  }
  if ((longAt(stackPointer)) == falseObj) {
    fsFlag = 0;
    goto l1;
  }

  /* begin success: */
  if (!primFailCode) {
    primFailCode = 1;
  }
  fsFlag = null;
  /* end booleanValueOf: */
l1:

  /* begin stackIntegerValue: */
  integerPointer = longAt(stackPointer + (1 * BytesPerWord));
  if ((((integerPointer) & 7) == 1)) {
    h = (integerPointer >> 3);
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    h = 0;
  }

  /* begin stackIntegerValue: */
  integerPointer = longAt(stackPointer + (2 * BytesPerWord));
  if ((((integerPointer) & 7) == 1)) {
    w = (integerPointer >> 3);
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    w = 0;
  }

  /* begin stackIntegerValue: */
  integerPointer = longAt(stackPointer + (3 * BytesPerWord));
  if ((((integerPointer) & 7) == 1)) {
    d = (integerPointer >> 3);
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    d = 0;
  }
  if (primFailCode) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  okay = ioSetDisplayMode(w, h, d, fsFlag);
  if (primFailCode) {
    /* primitiveFailFor: */
    primFailCode = PrimErrOperationFailed;
    return;
  }

  /* begin methodReturnBool: */
  assert(!((failed())));
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            booleanObjectOf(okay));
  stackPointer = sp;
}