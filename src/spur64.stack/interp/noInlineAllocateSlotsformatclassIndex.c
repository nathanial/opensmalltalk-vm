/* Extracted from interp.c:40986 (function noInlineAllocateSlotsformatclassIndex). */

static NoDbgRegParms NeverInline sqInt
noInlineAllocateSlotsformatclassIndex(sqInt numSlots, sqInt formatField, sqInt classIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt newObj;
    usqInt numBytes;

	/* begin allocateSlots:format:classIndex: */
	if (numSlots >= (numSlotsMask())) {
		if ((((usqInt)(numSlots)) >> 56) > 0) {
			return null;
		}
		newObj = GIV(freeStart) + BaseHeaderSize;
		numBytes = (BaseHeaderSize + BaseHeaderSize) + (numSlots * BytesPerOop);
	}
	else {
		newObj = GIV(freeStart);
		numBytes = BaseHeaderSize + ((numSlots < 1
		? 8 /* allocationUnit */
		: numSlots * BytesPerOop));
	}
	if ((GIV(freeStart) + numBytes) > GIV(scavengeThreshold)) {
		if (numSlots <= ((1U << (fixedFieldsFieldWidth())) - 1)) {
			if (!GIV(needGCFlag)) {
				/* begin scheduleScavenge */
				GIV(needGCFlag) = 1;
				forceInterruptCheck();
			}
		}
		return allocateSlotsInOldSpacebytesformatclassIndex(numSlots, numBytes, formatField, classIndex);
	}
	if (numSlots >= (numSlotsMask())) {
		longAtput((void *)(GIV(freeStart)),((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) + numSlots);
		longAtput((void *)(newObj),((((((usqLong) (numSlotsMask()))) << (numSlotsFullShift()))) + ((((usqInt)(formatField) << (formatShift()))))) + classIndex);
	}
	else {
		longAtput((void *)(newObj),((((((usqLong) numSlots)) << (numSlotsFullShift()))) + ((((usqInt)(formatField) << (formatShift()))))) + classIndex);
	}

	/* for header parsing we put a saturated slot count in the prepended overflow size word */
	assert((numBytes % (allocationUnit())) == 0);
	assert((newObj % (allocationUnit())) == 0);
	GIV(freeStart) += numBytes;
	return newObj;
}