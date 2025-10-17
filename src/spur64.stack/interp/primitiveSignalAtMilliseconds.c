/* Extracted from interp.c:69486 (function primitiveSignalAtMilliseconds). */

/*	Cause the time semaphore, if one has been registered, to be
	signalled when the microsecond clock is greater than or equal to
	the given tick value. A tick value of zero turns off timer interrupts. */

	/* StackInterpreterPrimitives>>#primitiveSignalAtMilliseconds */

static void
primitiveSignalAtMilliseconds(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqLong deltaMsecs;
    sqLong limit;
    sqLong msecs;
    sqInt msecsObj;
    sqInt sema;

	msecsObj = longAt(GIV(stackPointer));
	sema = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	msecs = positive32BitValueOf(msecsObj);
	if (!GIV(primFailCode)) {
		if (/* isSemaphoreOop: */
			((!(sema & (tagMask()))))
		 && (((longAt((void *)(sema))) & (classIndexMask())) == (rawHashBitsOf(longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassSemaphore) << (shiftForWord())))))))))) {
			/* begin splObj:put: */
			/* begin storePointer:ofObject:withValue: */
			assert(validStorePointerArgs(TheTimerSemaphore, GIV(specialObjectsOop), sema));
			assert(isNonImmediate(GIV(specialObjectsOop)));
			if (oopisGreaterThanOrEqualTo(GIV(specialObjectsOop), GIV(oldSpaceStart))) {
				if (/* isYoung: */
					((!(sema & (tagMask()))))
				 && (oopisLessThan(sema, GIV(oldSpaceStart)))) {
					/* begin possibleRootStoreInto: */
					if (!((byteAt((void *)(GIV(specialObjectsOop) + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
						remember(GIV(specialObjectsOop));
					}
				}
			}

			/* most stores into young objects */
			longAtput((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheTimerSemaphore) << (shiftForWord()))))),sema);
			deltaMsecs = msecs - ((ioMSecs()) & MillisecondClockMask);
			limit = ((usqInt)(MillisecondClockMask)) >> 1;

			/* Handle a roll-over that could happen in between image invocation of ioMSecs and this invocation.
			   This will limit the maximum relative duration to MillisecondClockMask/2, about 3 days currently.
			   Every delay longer than that limit may lead to undefined behavior (shorten delay, or no delay at all) */
			if (deltaMsecs > limit) {
				deltaMsecs -= MillisecondClockMask;
			}
			GIV(nextWakeupUsecs) = (deltaMsecs > 0
						? (ioUTCMicroseconds()) + (deltaMsecs * 1000)
						: ioUTCMicroseconds());

			/* begin pop: */
			GIV(stackPointer) += 2 * BytesPerWord;
			return;
		}
		if (sema == GIV(nilObj)) {
			/* begin storePointerUnchecked:ofObject:withValue: */
			assert((isNonImmediate(GIV(specialObjectsOop)))
			 && (!(isForwarded(GIV(specialObjectsOop)))));
			assert(validStorePointerUncheckedArgs(TheTimerSemaphore, GIV(specialObjectsOop), GIV(nilObj)));
			longAtput((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheTimerSemaphore) << (shiftForWord()))))),GIV(nilObj));
			GIV(nextWakeupUsecs) = 0;

			/* begin pop: */
			GIV(stackPointer) += 2 * BytesPerWord;
			return;
		}
	}

	/* primitiveFailFor: */
	GIV(primFailCode) = PrimErrBadArgument;
}