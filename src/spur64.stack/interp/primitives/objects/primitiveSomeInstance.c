/* Extracted from interp.c:24047 (function primitiveSomeInstance). */

/* InterpreterPrimitives>>#primitiveSomeInstance */

static void primitiveSomeInstance(void) {
  sqInt class;
  sqInt instance;

  class = longAt(stackPointer);

  /* For the mirror prims check that the class obj is actually a valid class. */
  instance = initialInstanceOf(class);
  if (instance) {
    /* begin pop:thenPush: */
    popthenPush(argumentCount + 1, instance);
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
}