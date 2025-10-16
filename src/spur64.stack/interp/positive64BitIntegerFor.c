/* Extracted from interp.c:58348 (function positive64BitIntegerFor). */

sqInt
positive64BitIntegerFor(usqLong integerValue)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt newLargeInteger;
    usqInt newObj;
    usqInt numBytes;
    sqInt numSlots;
    sqInt objFormat;
    sqInt sz;

	if (integerValue <= (MaxSmallInteger)) {
		return ((integerValue << 3) | 1);
	}
	sz = 8;
	objFormat = (firstByteFormat()) + ((8 - sz) & (BytesPerWord - 1));
	numSlots = 8 / BytesPerOop;

	/* begin eeInstantiateSmallClassIndex:format:numSlots: */
	assert((numSlots >= 0)
	 && ((knownClassAtIndex(ClassLargePositiveIntegerCompactIndex)) != GIV(nilObj)));
	assert(((objFormat < (firstByteFormat())
			? objFormat
			: objFormat & (byteFormatMask()))) == (instSpecOfClass(knownClassAtIndex(ClassLargePositiveIntegerCompactIndex))));

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
	long64Atput((void *)(newObj),((((((usqLong) numSlots)) << (numSlotsFullShift()))) + ((((usqInt)(objFormat) << (formatShift()))))) + ClassLargePositiveIntegerCompactIndex);
	GIV(freeStart) += numBytes;
	newLargeInteger = newObj;
	/* end eeInstantiateSmallClassIndex:format:numSlots: */
l1:

	/* storeLong64:ofObject:withValue: */
	long64Atput((void *)((newLargeInteger + BaseHeaderSize)),SQ_SWAP_8_BYTES_IF_BIGENDIAN(integerValue));
	return newLargeInteger;
}