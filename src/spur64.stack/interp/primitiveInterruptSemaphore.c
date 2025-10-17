/* Extracted from interp.c:18426 (function primitiveInterruptSemaphore). */

/*	Register the user interrupt semaphore. If the argument is 
	not a Semaphore, unregister the current interrupt 
	semaphore. */

	/* InterpreterPrimitives>>#primitiveInterruptSemaphore */

static void
primitiveInterruptSemaphore(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arg;

	arg = longAt(GIV(stackPointer));
	if ((arg == GIV(nilObj))
	 || (/* isSemaphoreOop: */
		((!(arg & (tagMask()))))
	 && (((longAt((void *)(arg))) & (classIndexMask())) == (rawHashBitsOf(longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassSemaphore) << (shiftForWord()))))))))))) {
		/* begin splObj:put: */
		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(TheInterruptSemaphore, GIV(specialObjectsOop), arg));
		assert(isNonImmediate(GIV(specialObjectsOop)));
		if (oopisGreaterThanOrEqualTo(GIV(specialObjectsOop), GIV(oldSpaceStart))) {
			if (/* isYoung: */
				((!(arg & (tagMask()))))
			 && (oopisLessThan(arg, GIV(oldSpaceStart)))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(GIV(specialObjectsOop) + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(GIV(specialObjectsOop));
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheInterruptSemaphore) << (shiftForWord()))))),arg);

		/* begin pop: */
		GIV(stackPointer) += 1 * BytesPerWord;
	}
	else {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
	}
}