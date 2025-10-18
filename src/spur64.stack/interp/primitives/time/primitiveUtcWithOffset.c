/* Extracted from interp.c:25718 (function primitiveUtcWithOffset). */

/*	Answer an array with UTC microseconds since the Posix epoch and the
        current seconds offset from GMT in the local time zone. An empty two
        element array (or any object with two or more slots) may be supplied
        as a parameter.
        This is a named (not numbered) primitive in the null module (ie the VM)
 */

/* InterpreterPrimitives>>#primitiveUtcWithOffset */

EXPORT(sqInt)
primitiveUtcWithOffset(void) {
  static usqLong epochDelta = 2177452800000000ULL;
  sqInt resultArray;
  char *sp;
  sqInt valuePointer;

  if (argumentCount > 0) {
    if (argumentCount > 1) {
      return (primFailCode = PrimErrBadNumArgs);
    }
    resultArray = longAt(stackPointer);
    if (!((/* isPointers: */
           ((!(resultArray & (tagMask())))) &&
           (((byteAt((void *)(resultArray + (formatFieldByteOffset())))) &
             (formatMask())) <= 5 /* lastPointerFormat */)) &&
          ((lengthOf(resultArray)) >= 2))) {
      return (primFailCode = PrimErrBadArgument);
    }
  } else {
    resultArray = instantiateClassindexableSize(
        fetchPointerofObject(ClassArray, specialObjectsOop), 2);
  }

  /* N.B. No pushRemappableOop:/popRemappableOop in Cog because
     positive64BitIntegerFor: et al use eeInstantiate... allocators which are
     guaranteed not to do a GC. */
  valuePointer = (((usqInt)(ioLocalSecondsOffset()) << 3) | 1);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(resultArray)) && (!(isForwarded(resultArray))));
  assert(validStorePointerUncheckedArgs(1, resultArray, valuePointer));
  longAtput((void *)((resultArray + BaseHeaderSize) + (1U << (shiftForWord()))),
            valuePointer);
  valuePointer = positive64BitIntegerFor((ioUTCMicrosecondsNow()) - epochDelta);

  storePointerofObjectwithValue(0, resultArray, valuePointer);

  /* begin pop:thenPush: */
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            resultArray);
  stackPointer = sp;
  return 0;
}