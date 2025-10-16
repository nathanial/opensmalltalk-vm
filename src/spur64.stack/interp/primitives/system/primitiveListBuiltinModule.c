/* Extracted from interp.c:18821 (function primitiveListBuiltinModule). */

/*	Primitive. Return the n-th builtin module name. */

/* InterpreterPrimitives>>#primitiveListBuiltinModule */

static void primitiveListBuiltinModule(void) {
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
  if ((moduleName = ioListBuiltinModule(index))) {
    methodReturnString(moduleName);
  } else {
    /* begin methodReturnValue: */
    assert(!((failed())));
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              nilObj);
    stackPointer = sp;
  }
}