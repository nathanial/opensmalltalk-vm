/* Extracted from interp.c:69560 (function primitiveSignalAtUTCMicroseconds). */

/*	Cause the time semaphore, if one has been registered, to be
        signalled when the microsecond clock is greater than or equal to
        the given tick value. A tick value of zero turns off timer interrupts.
 */

/* StackInterpreterPrimitives>>#primitiveSignalAtUTCMicroseconds */

static void primitiveSignalAtUTCMicroseconds(void) {
  sqInt sema;
  usqLong usecs;
  sqInt usecsObj;

  usecsObj = longAt(stackPointer);
  sema = longAt(stackPointer + (1 * BytesPerWord));
  usecs = positive64BitValueOf(usecsObj);
  if (!primFailCode) {
    if (/* isSemaphoreOop: */
        ((!(sema & (tagMask())))) &&
        (((longAt((void *)(sema))) & (classIndexMask())) ==
         (rawHashBitsOf(
             fetchPointerofObject(ClassSemaphore, specialObjectsOop))))) {
      /* begin splObj:put: */
      /* begin storePointer:ofObject:withValue: */
      assert(validStorePointerArgs(TheTimerSemaphore, specialObjectsOop, sema));
      assert(isNonImmediate(specialObjectsOop));
      if (oopisGreaterThanOrEqualTo(specialObjectsOop, oldSpaceStart)) {
        if (/* isYoung: */
            ((!(sema & (tagMask())))) && (oopisLessThan(sema, oldSpaceStart))) {
          /* begin possibleRootStoreInto: */
          if (!((byteAt(
                    (void *)(specialObjectsOop + (formatFieldByteOffset())))) &
                (1U << (rememberedBitByteShift())))) {
            remember(specialObjectsOop);
          }
        }
      }

      /* most stores into young objects */
      longAtput((void *)((specialObjectsOop + BaseHeaderSize) +
                         ((((usqInt)(TheTimerSemaphore) << (shiftForWord()))))),
                sema);
      nextWakeupUsecs = usecs;

      /* begin pop: */
      stackPointer += 2 * BytesPerWord;
      return;
    }
    if (sema == nilObj) {
      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(specialObjectsOop)) &&
             (!(isForwarded(specialObjectsOop))));
      assert(validStorePointerUncheckedArgs(TheTimerSemaphore,
                                            specialObjectsOop, nilObj));
      longAtput((void *)((specialObjectsOop + BaseHeaderSize) +
                         ((((usqInt)(TheTimerSemaphore) << (shiftForWord()))))),
                nilObj);
      nextWakeupUsecs = 0;

      /* begin pop: */
      stackPointer += 2 * BytesPerWord;
      return;
    }
  }

  /* primitiveFailFor: */
  primFailCode = PrimErrBadArgument;
}