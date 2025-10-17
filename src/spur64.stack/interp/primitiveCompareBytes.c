/* Extracted from interp.c:14194 (function primitiveCompareBytes). */

/*	Primitive. Compare two byte-indexed objects for equality */

/* InterpreterPrimitives>>#primitiveCompareBytes */

EXPORT(sqInt)
primitiveCompareBytes(void) {
  sqInt arg1;
  sqInt arg2;
  sqInt fmt;
  sqInt i;
  sqInt len;
  usqInt numSlots;
  char *sp;

  arg1 = longAt(stackPointer + (1 * BytesPerWord));
  arg2 = longAt(stackPointer);

  /* Quick identity test */
  if (arg1 == arg2) {
    /* begin methodReturnBool: */
    assert(!((failed())));
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              trueObj);
    stackPointer = sp;
    return 0;
  }
  if (!((/* isBytes: */
         ((!(arg1 & (tagMask())))) &&
         (((byteAt((void *)(arg1 + (formatFieldByteOffset())))) &
           (formatMask())) >= (firstByteFormat()))) &&
        (/* isBytes: */
         ((!(arg2 & (tagMask())))) &&
         (((byteAt((void *)(arg2 + (formatFieldByteOffset())))) &
           (formatMask())) >= (firstByteFormat()))))) {
    return (primFailCode = PrimErrBadArgument);
  }

  /* begin numBytesOfBytes: */
  fmt = (byteAt((void *)(arg1 + (formatFieldByteOffset())))) & (formatMask());
  assert(fmt >= (firstByteFormat()));
  len =
      ((((/* begin numSlotsOf: */
          assert((classIndexOf(arg1)) > (isForwardedObjectClassIndexPun())),
          (((numSlots =
                 byteAt((void *)(arg1 + (numSlotsFieldByteOffset()))))) ==
                   (numSlotsMask())
               ? ((((usqInt)((
                     (sqInt)((usqInt)((longAt((void *)(arg1 - BaseHeaderSize))))
                             << 8)))))) >>
                     8
               : numSlots)))
        << (shiftForWord()))) -
      (fmt & 7);
  if (!(len == (numBytesOfBytes(arg2)))) {
    /* begin methodReturnBool: */
    assert(!((failed())));
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              falseObj);
    stackPointer = sp;
    return 0;
  }
  for (i = 0; i < len; i += 1) {
    if (!((byteAt((void *)((arg1 + BaseHeaderSize) + i))) ==
          (byteAt((void *)((arg2 + BaseHeaderSize) + i))))) {
      /* begin methodReturnBool: */
      assert(!((failed())));
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          falseObj);
      stackPointer = sp;
      return 0;
    }
  }

  /* begin methodReturnBool: */
  assert(!((failed())));
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            trueObj);
  stackPointer = sp;
  return 0;
}