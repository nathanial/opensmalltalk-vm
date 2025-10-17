/* Extracted from interp.c:63404 (function signed64BitIntegerFor). */

/*	Answer a Large Integer object for the given integer value. N.B. will *not*
	cause a GC. */

	/* StackInterpreter>>#signed64BitIntegerFor: */

sqInt
signed64BitIntegerFor(sqLong integerValue)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt largeClass;
    usqLong magnitude;
    usqInt newLargeInteger;
    usqInt newObj;
    usqInt numBytes;
    sqInt numSlots;
    sqInt objFormat;
    sqInt sz;

	if (integerValue < 0) {
		if (integerValue >= (MinSmallInteger)) {
			return (((usqInt)(((sqInt)integerValue)) << 3) | 1);
		}
		largeClass = ClassLargeNegativeIntegerCompactIndex;
		magnitude = 0 - (((usqLong) integerValue));
	}
	else {
		if (integerValue <= (MaxSmallInteger)) {
			return (((usqInt)(((sqInt)integerValue)) << 3) | 1);
		}
		largeClass = ClassLargePositiveIntegerCompactIndex;
		magnitude = integerValue;
	}
	sz = 8;
	objFormat = (firstByteFormat()) + ((8 - sz) & (BytesPerWord - 1));
	numSlots = (sz + 3) / BytesPerOop;

	/* begin eeInstantiateSmallClassIndex:format:numSlots: */
	assert((numSlots >= 0)
	 && ((largeClass != 0)
	 && ((knownClassAtIndex(largeClass)) != GIV(nilObj))));
	assert(((objFormat < (firstByteFormat())
			? objFormat
			: objFormat & (byteFormatMask()))) == (instSpecOfClass(knownClassAtIndex(largeClass))));

	/* begin allocateSmallNewSpaceSlots:format:classIndex: */
	assert(numSlots < (numSlotsMask()));
	newObj = GIV(freeStart);
	numBytes = BaseHeaderSize + ((numSlots < 1
		? 8 /* allocationUnit */
		: numSlots * BytesPerOop));
	assert((numBytes % (allocationUnit())) == 0);
	assert((newObj % (allocationUnit())) == 0);
	if ((GIV(freeStart) + numBytes) > GIV(scavengeThreshold)) {
		if (!GIV(needGCFlag)) {
			/* begin scheduleScavenge */
			GIV(needGCFlag) = 1;
			forceInterruptCheck();
		}
		if ((GIV(freeStart) + numBytes) > (((GIV(eden)).limit))) {
			error("no room in eden for allocateSmallNewSpaceSlots:format:classIndex:");
			newLargeInteger = 0;
			goto l1;
		}
	}
	long64Atput((void *)(newObj),((((((usqLong) numSlots)) << (numSlotsFullShift()))) + ((((usqInt)(objFormat) << (formatShift()))))) + largeClass);
	GIV(freeStart) += numBytes;
	newLargeInteger = newObj;
	/* end eeInstantiateSmallClassIndex:format:numSlots: */
l1:

	/* storeLong64:ofObject:withValue: */
	long64Atput((void *)((newLargeInteger + BaseHeaderSize)),SQ_SWAP_8_BYTES_IF_BIGENDIAN(magnitude));

	/* Memory is eight byte aligned in SPUR, so we are sure to have room for 64bits word whatever allocated sz */
	return newLargeInteger;
}