/* Extracted from interp.c:69560 (function primitiveSignalAtUTCMicroseconds). */

/*	Cause the time semaphore, if one has been registered, to be
	signalled when the microsecond clock is greater than or equal to
	the given tick value. A tick value of zero turns off timer interrupts. */

	/* StackInterpreterPrimitives>>#primitiveSignalAtUTCMicroseconds */

static void
primitiveSignalAtUTCMicroseconds(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt sema;
    usqLong usecs;
    sqInt usecsObj;

	usecsObj = longAt(GIV(stackPointer));
	sema = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	usecs = positive64BitValueOf(usecsObj);
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
			GIV(nextWakeupUsecs) = usecs;

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