/* Extracted from interp.c:69486 (function primitiveSignalAtMilliseconds). */

/*	Cause the time semaphore, if one has been registered, to be
        signalled when the microsecond clock is greater than or equal to
        the given tick value. A tick value of zero turns off timer interrupts.
 */

/* StackInterpreterPrimitives>>#primitiveSignalAtMilliseconds */

static void primitiveSignalAtMilliseconds(void) {
  sqLong deltaMsecs;
  sqLong limit;
  sqLong msecs;
  sqInt msecsObj;
  sqInt sema;

  msecsObj = longAt(stackPointer);
  sema = longAt(stackPointer + (1 * BytesPerWord));
  msecs = positive32BitValueOf(msecsObj);
  if (!primFailCode) {
    if (/* isSemaphoreOop: */
        ((!(sema & (tagMask())))) &&
        (((longAt((void *)(sema))) & (classIndexMask())) ==
         (rawHashBitsOf(longAt(
             (void *)((specialObjectsOop + BaseHeaderSize) +
                      ((((usqInt)(ClassSemaphore) << (shiftForWord())))))))))) {
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
      deltaMsecs = msecs - ((ioMSecs()) & MillisecondClockMask);
      limit = ((usqInt)(MillisecondClockMask)) >> 1;

      /* Handle a roll-over that could happen in between image invocation of
         ioMSecs and this invocation. This will limit the maximum relative
         duration to MillisecondClockMask/2, about 3 days currently. Every delay
         longer than that limit may lead to undefined behavior (shorten delay,
         or no delay at all) */
      if (deltaMsecs > limit) {
        deltaMsecs -= MillisecondClockMask;
      }
      nextWakeupUsecs =
          (deltaMsecs > 0 ? (ioUTCMicroseconds()) + (deltaMsecs * 1000)
                          : ioUTCMicroseconds());

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