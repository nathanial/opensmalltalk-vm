/* Extracted from interp.c:11709 (function primitiveAdoptInstance). */

/*	Primitive. Change the class of the argument to make it an instance of
   the receiver given that the format of the receiver matches the format of the
        argument's class.
        Fail if receiver or argument are SmallIntegers, or the receiver is an
        instance of a
        compact class and the argument isn't, or when the argument's class is
        compact and
        the receiver isn't, or when the format of the receiver is different from
        the format of
        the argument's class, or when the arguments class is fixed and the
        receiver's size
        differs from the size that an instance of the argument's class should
        have.
 */

/* InterpreterPrimitives>>#primitiveAdoptInstance */

static void primitiveAdoptInstance(void) {
  sqInt arg;
  sqInt err;
  sqInt rcvr;

  arg = longAt(stackPointer);
  rcvr = longAt(stackPointer + (1 * BytesPerWord));
  if ((((arg & (tagMask())) != 0)) ||
      ((argumentCount > 1) &&
       ((((rcvr & (tagMask())) != 0)) || (!(objCouldBeClassObj(rcvr)))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  err = changeClassOfto(arg, rcvr);
  if (err) {
    if (err == PrimErrBadReceiver) {
      err = PrimErrBadArgument;
    } else {
      if (err == PrimErrBadArgument) {
        err = PrimErrBadReceiver;
      }
    }

    /* primitiveFailFor: */
    primFailCode = err;
  } else {
    /* begin flushAtCache */
    memset(atCache, 0, AtCacheTotalSize * (sizeof(atCache[0])));
    stackPointer += argumentCount * BytesPerWord;
  }

  /* Flush at cache because rcvr's class has changed.
     changeClassOf:to: answers errors as if rcvr (the class) is an argument...
   */
  return;
}