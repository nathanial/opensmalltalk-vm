/* Extracted from interp.c:13537 (function primitiveClockLogAddresses). */

EXPORT(sqInt)
primitiveClockLogAddresses(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
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
	msecs = ((void *) 0);
	uidx = 0;
	usecs = ((void *) 0);
	if (GIV(argumentCount) != 1) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return null;
	}
	runInNOut = (longAt(GIV(stackPointer))) == GIV(trueObj);
	ioGetClockLogSizeUsecsIdxMsecsIdx((&runInNOut), (&usecs), (&uidx), (&msecs), (&midx));
	result = instantiateClassindexableSize(longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassArray) << (shiftForWord())))))), 5);
	if (!result) {
		return (GIV(primFailCode) = PrimErrNoMemory);
	}
	
#if SPURVM
	v1 = positive64BitIntegerFor(((usqInt)usecs));
		v2 = positive64BitIntegerFor(((usqInt)msecs));
#else /* SPURVM */
	pushRemappableOop(result);
	v1 = positive64BitIntegerFor(((usqInt)usecs));
		pushRemappableOop(v1);
	v2 = positive64BitIntegerFor(((usqInt)msecs));
	v1 = popRemappableOop();
	result = popRemappableOop();
#endif /* SPURVM */
	if (GIV(primFailCode)) {
		return (GIV(primFailCode) = PrimErrNoMemory);
	}

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(result))
	 && (!(isForwarded(result))));
	assert(validStorePointerUncheckedArgs(0, result, (((usqInt)runInNOut << 3) | 1)));
	longAtput((void *)((result + BaseHeaderSize) + (0U << (shiftForWord()))),(((usqInt)runInNOut << 3) | 1));

	/* begin storePointer:ofObject:withValue: */
	assert(validStorePointerArgs(1, result, v1));
	assert(isNonImmediate(result));
	if (oopisGreaterThanOrEqualTo(result, GIV(oldSpaceStart))) {
		if (/* isYoung: */
			((!(v1 & (tagMask()))))
		 && (oopisLessThan(v1, GIV(oldSpaceStart)))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(result + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(result);
			}
		}
	}

	/* most stores into young objects */
	longAtput((void *)((result + BaseHeaderSize) + (1U << (shiftForWord()))),v1);

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(result))
	 && (!(isForwarded(result))));
	assert(validStorePointerUncheckedArgs(2, result, (((usqInt)uidx << 3) | 1)));
	longAtput((void *)((result + BaseHeaderSize) + (2U << (shiftForWord()))),(((usqInt)uidx << 3) | 1));

	/* begin storePointer:ofObject:withValue: */
	assert(validStorePointerArgs(3, result, v2));
	assert(isNonImmediate(result));
	if (oopisGreaterThanOrEqualTo(result, GIV(oldSpaceStart))) {
		if (/* isYoung: */
			((!(v2 & (tagMask()))))
		 && (oopisLessThan(v2, GIV(oldSpaceStart)))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(result + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(result);
			}
		}
	}

	/* most stores into young objects */
	longAtput((void *)((result + BaseHeaderSize) + (3U << (shiftForWord()))),v2);

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(result))
	 && (!(isForwarded(result))));
	assert(validStorePointerUncheckedArgs(4, result, (((usqInt)midx << 3) | 1)));
	longAtput((void *)((result + BaseHeaderSize) + (4U << (shiftForWord()))),(((usqInt)midx << 3) | 1));

	/* begin methodReturnValue: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),result);
	GIV(stackPointer) = sp;
	return 0;
}