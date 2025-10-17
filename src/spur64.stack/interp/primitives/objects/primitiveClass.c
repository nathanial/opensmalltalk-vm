/* Extracted from interp.c:13423 (function primitiveClass). */

/* InterpreterPrimitives>>#primitiveClass */

static void primitiveClass(void) {
  sqInt instance;
  sqInt oop;
  char *sp;
  sqInt tagBits;

  instance = longAt(stackPointer);
  if ((argumentCount > 0) && (isOopForwarded(instance))) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  } else {
    oop = /* fetchClassOf: */
        ((tagBits = instance & (tagMask()))
             ? longAt((void *)((classTableFirstPage + BaseHeaderSize) +
                               ((((usqInt)(tagBits) << (shiftForWord()))))))
             : fetchClassOfNonImm(instance));

    /* begin pop:thenPush: */
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              oop);
    stackPointer = sp;
  }
}