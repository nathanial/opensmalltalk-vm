/* Extracted from interp.c:13537 (function primitiveClockLogAddresses). */

/*	Take a boolean which if true turns or keeps clock logging on. Answer an
        array supplying
        the size of the clock logs, the address of the usecs log, the index in
   it, the address of the msecs log, and the index into it. */

/* InterpreterPrimitives>>#primitiveClockLogAddresses */

EXPORT(sqInt)
primitiveClockLogAddresses(void) {
  sqInt midx;
  void *msecs;
  sqInt result;
  sqInt runInNOut;
  char *sp;
  sqInt uidx;
  void *usecs;
  sqInt v1;
  sqInt v2;

  /* bypass type inference which would deduce int */
  midx = 0;
  msecs = ((void *)0);
  uidx = 0;
  usecs = ((void *)0);
  if (argumentCount != 1) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return null;
  }
  runInNOut = (longAt(stackPointer)) == trueObj;
  ioGetClockLogSizeUsecsIdxMsecsIdx((&runInNOut), (&usecs), (&uidx), (&msecs),
                                    (&midx));
  result = instantiateClassindexableSize(
      longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                      ((((usqInt)(ClassArray) << (shiftForWord())))))),
      5);
  if (!result) {
    return (primFailCode = PrimErrNoMemory);
  }

#if SPURVM
  v1 = positive64BitIntegerFor(((usqInt)usecs));
  v2 = positive64BitIntegerFor(((usqInt)msecs));
#else  /* SPURVM */
  pushRemappableOop(result);
  v1 = positive64BitIntegerFor(((usqInt)usecs));
  pushRemappableOop(v1);
  v2 = positive64BitIntegerFor(((usqInt)msecs));
  v1 = popRemappableOop();
  result = popRemappableOop();
#endif /* SPURVM */
  if (primFailCode) {
    return (primFailCode = PrimErrNoMemory);
  }

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(result)) && (!(isForwarded(result))));
  assert(validStorePointerUncheckedArgs(0, result,
                                        (((usqInt)runInNOut << 3) | 1)));
  longAtput((void *)((result + BaseHeaderSize) + (0U << (shiftForWord()))),
            (((usqInt)runInNOut << 3) | 1));

  /* begin storePointer:ofObject:withValue: */
  assert(validStorePointerArgs(1, result, v1));
  assert(isNonImmediate(result));
  if (oopisGreaterThanOrEqualTo(result, oldSpaceStart)) {
    if (/* isYoung: */
        ((!(v1 & (tagMask())))) && (oopisLessThan(v1, oldSpaceStart))) {
      /* begin possibleRootStoreInto: */
      if (!((byteAt((void *)(result + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift())))) {
        remember(result);
      }
    }
  }

  /* most stores into young objects */
  longAtput((void *)((result + BaseHeaderSize) + (1U << (shiftForWord()))), v1);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(result)) && (!(isForwarded(result))));
  assert(validStorePointerUncheckedArgs(2, result, (((usqInt)uidx << 3) | 1)));
  longAtput((void *)((result + BaseHeaderSize) + (2U << (shiftForWord()))),
            (((usqInt)uidx << 3) | 1));

  /* begin storePointer:ofObject:withValue: */
  assert(validStorePointerArgs(3, result, v2));
  assert(isNonImmediate(result));
  if (oopisGreaterThanOrEqualTo(result, oldSpaceStart)) {
    if (/* isYoung: */
        ((!(v2 & (tagMask())))) && (oopisLessThan(v2, oldSpaceStart))) {
      /* begin possibleRootStoreInto: */
      if (!((byteAt((void *)(result + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift())))) {
        remember(result);
      }
    }
  }

  /* most stores into young objects */
  longAtput((void *)((result + BaseHeaderSize) + (3U << (shiftForWord()))), v2);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(result)) && (!(isForwarded(result))));
  assert(validStorePointerUncheckedArgs(4, result, (((usqInt)midx << 3) | 1)));
  longAtput((void *)((result + BaseHeaderSize) + (4U << (shiftForWord()))),
            (((usqInt)midx << 3) | 1));

  /* begin methodReturnValue: */
  assert(!((failed())));
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            result);
  stackPointer = sp;
  return 0;
}