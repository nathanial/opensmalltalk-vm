/* Extracted from interp.c:58469 (function primitiveEventProcessingControl). */

/*	With 0 args answers whether ioProcessEvents is enabled and being called.
        With 1 arg expects a boolean which will enable ioProcessEvents if true
   and disable it if false, answering its previous state. */

/* StackInterpreter>>#primitiveEventProcessingControl */

EXPORT(void)
primitiveEventProcessingControl(void) {
  int enabled;
  char *sp;

  enabled = inIOProcessEvents >= 0;
  if (!argumentCount) {
    /* begin pop:thenPushBool: */
    longAtput((sp = stackPointer), /* booleanObjectOf: */
              (enabled ? trueObj : falseObj));
    stackPointer = sp;
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
    longAtput((sp = stackPointer + (1 * BytesPerWord)), /* booleanObjectOf: */
              (enabled ? trueObj : falseObj));
    stackPointer = sp;
    return;
  }

  /* primitiveFailFor: */
  primFailCode = PrimErrBadNumArgs;
}