/* Extracted from interp.c:25650 (function primitiveUtcAndTimezoneOffset). */

/*	Answer an array with UTC microseconds since the Smalltalk epoch and the
        current seconds offset from GMT in the local time zone. Any pointer
   object with two or more slots) may be supplied as a parameter. */

/* InterpreterPrimitives>>#primitiveUtcAndTimezoneOffset */

static void primitiveUtcAndTimezoneOffset(void) {
  sqInt resultArray;
  char *sp;
  sqInt valuePointer;

  if (argumentCount > 0) {
    if (argumentCount > 1) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadNumArgs;
      return;
    }
    resultArray = longAt(stackPointer);
    if (!((/* isPointers: */
           ((!(resultArray & (tagMask())))) &&
           (((byteAt((void *)(resultArray + (formatFieldByteOffset())))) &
             (formatMask())) <= 5 /* lastPointerFormat */)) &&
          ((lengthOf(resultArray)) >= 2))) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
      return;
    }
  } else {
    resultArray = instantiateClassindexableSize(
        fetchPointerofObject(ClassArray, specialObjectsOop),
        2);
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
  valuePointer = positive64BitIntegerFor(ioUTCMicroseconds());

  /* begin storePointer:ofObject:withValue: */
  assert(validStorePointerArgs(0, resultArray, valuePointer));
  assert(isNonImmediate(resultArray));
  if (oopisGreaterThanOrEqualTo(resultArray, oldSpaceStart)) {
    if (/* isYoung: */
        ((!(valuePointer & (tagMask())))) &&
        (oopisLessThan(valuePointer, oldSpaceStart))) {
      /* begin possibleRootStoreInto: */
      if (!((byteAt((void *)(resultArray + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift())))) {
        remember(resultArray);
      }
    }
  }

  /* most stores into young objects */
  longAtput((void *)((resultArray + BaseHeaderSize) + (0U << (shiftForWord()))),
            valuePointer);

  /* begin pop:thenPush: */
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            resultArray);
  stackPointer = sp;
}