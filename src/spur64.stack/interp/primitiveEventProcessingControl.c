/* Extracted from interp.c:58469 (function primitiveEventProcessingControl). */

/*	With 0 args answers whether ioProcessEvents is enabled and being called.
        With 1 arg expects a boolean which will enable ioProcessEvents if true
   and disable it if false, answering its previous state. */

/* StackInterpreter>>#primitiveEventProcessingControl */

EXPORT(void)
primitiveEventProcessingControl(void) {
  int enabled;

  enabled = inIOProcessEvents >= 0;
  if (!argumentCount) {
    /* begin pop:thenPushBool: */
    popthenPushBool(1, booleanObjectOf(enabled));
    return;
  }
  if (argumentCount == 1) {
    if ((longAt(stackPointer)) == trueObj) {
      if (inIOProcessEvents < 0) {
        inIOProcessEvents = 0;
      }
    } else {
      if ((longAt(stackPointer)) == falseObj) {
        inIOProcessEvents = -1;
      } else {
        /* primitiveFailFor: */
        primFailCode = PrimErrBadArgument;
        return;
      }
    }

    /* begin pop:thenPushBool: */
    popthenPushBool(2, booleanObjectOf(enabled));
    return;
  }

  /* primitiveFailFor: */
  primFailCode = PrimErrBadNumArgs;
}