/* Extracted from interp.c:25650 (function primitiveUtcAndTimezoneOffset). */

/*	Answer an array with UTC microseconds since the Smalltalk epoch and the
	current seconds offset from GMT in the local time zone. Any pointer object
	with two or more slots) may be supplied as a parameter. */

	/* InterpreterPrimitives>>#primitiveUtcAndTimezoneOffset */

static void
primitiveUtcAndTimezoneOffset(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt resultArray;
    char *sp;
    sqInt valuePointer;

	if (GIV(argumentCount) > 0) {
		if (GIV(argumentCount) > 1) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadNumArgs;
			return;
		}
		resultArray = longAt(GIV(stackPointer));
		if (!((/* isPointers: */
				((!(resultArray & (tagMask()))))
			 && (((byteAt((void *)(resultArray + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */))
			 && ((lengthOf(resultArray)) >= 2))) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadArgument;
			return;
		}
	}
	else {
		resultArray = instantiateClassindexableSize(longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassArray) << (shiftForWord())))))), 2);
	}

	/* N.B. No pushRemappableOop:/popRemappableOop in Cog because positive64BitIntegerFor: et al use
	   eeInstantiate... allocators which are guaranteed not to do a GC. */
	valuePointer = (((usqInt)(ioLocalSecondsOffset()) << 3) | 1);

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(resultArray))
	 && (!(isForwarded(resultArray))));
	assert(validStorePointerUncheckedArgs(1, resultArray, valuePointer));
	longAtput((void *)((resultArray + BaseHeaderSize) + (1U << (shiftForWord()))),valuePointer);
	valuePointer = positive64BitIntegerFor(ioUTCMicroseconds());

	/* begin storePointer:ofObject:withValue: */
	assert(validStorePointerArgs(0, resultArray, valuePointer));
	assert(isNonImmediate(resultArray));
	if (oopisGreaterThanOrEqualTo(resultArray, GIV(oldSpaceStart))) {
		if (/* isYoung: */
			((!(valuePointer & (tagMask()))))
		 && (oopisLessThan(valuePointer, GIV(oldSpaceStart)))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(resultArray + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(resultArray);
			}
		}
	}

	/* most stores into young objects */
	longAtput((void *)((resultArray + BaseHeaderSize) + (0U << (shiftForWord()))),valuePointer);

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),resultArray);
	GIV(stackPointer) = sp;
}