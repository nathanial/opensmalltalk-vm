/* Extracted from interp.c:11274 (function magnitude64BitIntegerForneg). */

static NoDbgRegParms sqInt
magnitude64BitIntegerForneg(usqLong magnitude, sqInt isNegative)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    int isSmall;
    int largeClassIndex;
    usqInt newLargeInteger;
    usqInt newObj;
    usqInt numBytesUsqInt;
    sqInt numSlots;
    sqInt objFormat;
    sqInt smallVal;
    sqInt sz;

	isSmall = (isNegative
				? magnitude <= ((MaxSmallInteger) + 1)
				: magnitude <= (MaxSmallInteger));
	if (isSmall) {
		smallVal = ((sqInt) magnitude);
		if (isNegative) {
			smallVal = 0 - smallVal;
		}
		return (((usqInt)smallVal << 3) | 1);
	}
	largeClassIndex = (isNegative
				? ClassLargeNegativeIntegerCompactIndex
				: ClassLargePositiveIntegerCompactIndex);
	sz = 8;
	objFormat = (firstByteFormat()) + ((8 - sz) & (BytesPerWord - 1));

	/* begin eeInstantiateSmallClassIndex:format:numBytes: */
	assert((sz >= 0)
	 && ((largeClassIndex != 0)
	 && ((knownClassAtIndex(largeClassIndex)) != GIV(nilObj))));
	assert(((objFormat < (firstByteFormat())
			? objFormat
			: objFormat & (byteFormatMask()))) == (instSpecOfClass(knownClassAtIndex(largeClassIndex))));
	numSlots = ((sz + BytesPerWord) - 1) / BytesPerWord;

	/* begin allocateSmallNewSpaceSlots:format:classIndex: */
	assert(numSlots < (numSlotsMask()));
	newObj = GIV(freeStart);
	numBytesUsqInt = BaseHeaderSize + ((numSlots < 1
		? 8 /* allocationUnit */
		: numSlots * BytesPerOop));
	assert((numBytesUsqInt % (allocationUnit())) == 0);
	assert((newObj % (allocationUnit())) == 0);
	if ((GIV(freeStart) + numBytesUsqInt) > GIV(scavengeThreshold)) {
		if (!GIV(needGCFlag)) {
			/* begin scheduleScavenge */
			GIV(needGCFlag) = 1;
			forceInterruptCheck();
		}
		if ((GIV(freeStart) + numBytesUsqInt) > (((GIV(eden)).limit))) {
			error("no room in eden for allocateSmallNewSpaceSlots:format:classIndex:");
			newLargeInteger = 0;
			goto l1;
		}
	}
	long64Atput((void *)(newObj),((((((usqLong) numSlots)) << (numSlotsFullShift()))) + ((((usqInt)(objFormat) << (formatShift()))))) + largeClassIndex);
	GIV(freeStart) += numBytesUsqInt;
	newLargeInteger = newObj;
	/* end eeInstantiateSmallClassIndex:format:numBytes: */
l1:

	/* storeLong64:ofObject:withValue: */
	long64Atput((void *)((newLargeInteger + BaseHeaderSize)),SQ_SWAP_8_BYTES_IF_BIGENDIAN(magnitude));

	/* Memory is eight byte aligned in SPUR, so we are sure to have room for 64bits word whatever allocated sz */
	return newLargeInteger;
}