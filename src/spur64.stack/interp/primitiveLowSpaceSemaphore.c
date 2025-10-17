/* Extracted from interp.c:19048 (function primitiveLowSpaceSemaphore). */

/*	Register the low-space semaphore. If the argument is not a 
	Semaphore, unregister the current low-space Semaphore. */

	/* InterpreterPrimitives>>#primitiveLowSpaceSemaphore */

static void
primitiveLowSpaceSemaphore(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arg;

	arg = longAt(stackPointer);
	if ((arg == nilObj)
	 || (/* isSemaphoreOop: */
		((!(arg & (tagMask()))))
	 && (((longAt((void *)(arg))) & (classIndexMask())) == (rawHashBitsOf(longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassSemaphore) << (shiftForWord()))))))))))) {
		/* begin splObj:put: */
		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(TheLowSpaceSemaphore, specialObjectsOop, arg));
		assert(isNonImmediate(specialObjectsOop));
		if (oopisGreaterThanOrEqualTo(specialObjectsOop, oldSpaceStart)) {
			if (/* isYoung: */
				((!(arg & (tagMask()))))
			 && (oopisLessThan(arg, oldSpaceStart))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(specialObjectsOop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(specialObjectsOop);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(TheLowSpaceSemaphore) << (shiftForWord()))))),arg);

		/* begin pop: */
		stackPointer += 1 * BytesPerWord;
	}
	else {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
	}
}