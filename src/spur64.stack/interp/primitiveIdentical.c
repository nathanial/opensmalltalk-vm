/* Extracted from interp.c:17734 (function primitiveIdentical). */

/*	is the receiver/first argument the same object as the (last) argument?.
        pop argumentCount because this can be used as a mirror primitive. */

/* InterpreterPrimitives>>#primitiveIdentical */

static void primitiveIdentical(void) {
  sqInt otherObject;
  sqInt thisObject;

  thisObject = longAt(stackPointer + (1 * BytesPerWord));
  otherObject = longAt(stackPointer);
  if ((/* isOopForwarded: */
       ((!(otherObject & (tagMask())))) &&
       ((!((longAt((void *)(otherObject))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) ||
      ((argumentCount > 1) &&
       (/* isOopForwarded: */
        ((!(thisObject & (tagMask())))) &&
        ((!((longAt((void *)(thisObject))) &
            ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
  } else {
    /* begin pop:thenPushBool: */
    popthenPushBool(argumentCount + 1, /* booleanObjectOf: */
                    (thisObject == otherObject ? trueObj : falseObj));
  }
}