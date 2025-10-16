/* Extracted from interp.c:17130 (function primitiveGetenv). */

EXPORT(sqInt)
primitiveGetenv(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *dstPtr;
    sqInt fmt;
    char *key;
    usqInt numBytes;
    usqInt numSlots;
    sqInt obj;
    char *sp;
    sqInt sz;
    char *val;

	if (sHEAFn) {
		if (!(sHEAFn())) {
			return (GIV(primFailCode) = PrimErrInappropriate);
		}
	}

	/* secHasEnvironmentAccess */

	/* begin stackEphemeralStringValue: */
	obj = longAt(GIV(stackPointer));
	if (!(/* isBytes: */
			((!(obj & (tagMask()))))
		 && (((byteAt((void *)(obj + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat())))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		key = null;
		goto l2;
	}

	/* begin byteSizeOf: */
	if (((obj & (tagMask())) != 0)) {
		sz = 0;
		goto l1;
	}

	/* begin numBytesOf: */
	fmt = (byteAt((void *)(obj + (formatFieldByteOffset())))) & (formatMask());
	assert((classIndexOf(obj)) > (isForwardedObjectClassIndexPun()));
	numBytes = (((numSlots = byteAt((void *)(obj + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(obj - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	numBytes = (numBytes << (shiftForWord()));
	if (fmt >= (firstByteFormat())) {
		sz = numBytes - (fmt & 7);
		goto l1;
	}

	/* bytes (the common case), including CompiledMethod */
	if (fmt <= (sixtyFourBitIndexableFormat())) {
		sz = numBytes;
		goto l1;
	}
	if (fmt >= (firstShortFormat())) {
		sz = numBytes - (((fmt & 3) << 1));
		goto l1;
	}

	/* fmt >= self firstLongFormat */
	sz = numBytes - (((fmt & 1) << 2));
	/* end byteSizeOf: */
l1:
	dstPtr = alloca(sz + 1);
	if (!dstPtr) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrNoCMemory;
		key = null;
		goto l2;
	}
	memcpy(dstPtr, ((char *) (firstIndexableField(obj))), sz);
	dstPtr[sz] = 0;
	key = dstPtr;
	/* end stackEphemeralStringValue: */
l2:
	if (key) {
		if ((val = getenv(key))) {
			methodReturnString(val);
		}
		else {
			/* begin methodReturnValue: */
			assert(!((failed())));
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),GIV(nilObj));
			GIV(stackPointer) = sp;
		}
	}
	return 0;
}