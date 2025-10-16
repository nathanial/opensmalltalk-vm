/* Extracted from interp.c:18872 (function primitiveListExternalModule). */

/*	Primitive. Answer the n-th loaded external module name. */

/* InterpreterPrimitives>>#primitiveListExternalModule */

static void primitiveListExternalModule(void) {
  sqInt index;
  sqInt integerPointer;
  char *moduleName;
  char *sp;

  if (!(argumentCount == 1)) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }

  /* begin stackIntegerValue: */
  integerPointer = longAt(stackPointer);
  if ((((integerPointer) & 7) == 1)) {
    index = (integerPointer >> 3);
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    index = 0;
  }
  if (primFailCode || (index <= 0)) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }
  if ((moduleName = ioListLoadedModule(index))) {
    methodReturnString(moduleName);
  } else {
    /* begin methodReturnValue: */
    assert(!((failed())));
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              nilObj);
    stackPointer = sp;
  }
}