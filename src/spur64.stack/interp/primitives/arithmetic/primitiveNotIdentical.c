/* Extracted from interp.c:19899 (function primitiveNotIdentical). */

/*	is the receiver/first argument not the same object as the (last)
        argument?. pop argumentCount because this can be used as a mirror
        primitive.
 */
/*	is the receiver/first argument the same object as the (last) argument?.
        pop argumentCount because this can be used as a mirror primitive. */

/* InterpreterPrimitives>>#primitiveNotIdentical */

static void primitiveNotIdentical(void) {
  sqInt otherObject;
  sqInt thisObject;

  thisObject = longAt(stackPointer + (1 * BytesPerWord));
  otherObject = longAt(stackPointer);
  if ((isOopForwarded(otherObject)) ||
      ((argumentCount > 1) && (isOopForwarded(thisObject)))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
  } else {
    /* begin pop:thenPushBool: */
    popthenPushBool(argumentCount + 1,
                    booleanObjectOf(thisObject != otherObject));
  }
}