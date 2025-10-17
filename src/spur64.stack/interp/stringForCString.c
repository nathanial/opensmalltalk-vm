/* Extracted from interp.c:46094 (function stringForCString). */

/*	Answer a new String copied from a null-terminated C string,
	or nil if out of memory. */

	/* SpurMemoryManager>>#stringForCString: */

sqInt
stringForCString(const char *aCString)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt formatField;
    sqInt len;
    usqInt newObj;
    sqInt newString;
    usqInt numBytes;
    sqInt numSlots;

	len = strlen(aCString);
	numSlots = (len + (BytesPerWord - 1)) / BytesPerWord;
	formatField = (firstByteFormat()) + ((8 - len) & (BytesPerWord - 1));

	/* begin allocateSlots:format:classIndex: */
	if (numSlots >= (numSlotsMask())) {
		if ((((usqInt)(numSlots)) >> 56) > 0) {
			newString = null;
			goto l1;
		}
		newObj = freeStart + BaseHeaderSize;
		numBytes = (BaseHeaderSize + BaseHeaderSize) + (numSlots * BytesPerOop);
	}
	else {
		newObj = freeStart;
		numBytes = BaseHeaderSize + ((numSlots < 1
		? 8 /* allocationUnit */
		: numSlots * BytesPerOop));
	}
	if ((freeStart + numBytes) > scavengeThreshold) {
		if (numSlots <= ((1U << (fixedFieldsFieldWidth())) - 1)) {
			if (!needGCFlag) {
				/* begin scheduleScavenge */
				needGCFlag = 1;
				forceInterruptCheck();
			}
		}
		newString = allocateSlotsInOldSpacebytesformatclassIndex(numSlots, numBytes, formatField, ClassByteStringCompactIndex);
		goto l1;
	}
	if (numSlots >= (numSlotsMask())) {
		longAtput((void *)(freeStart),((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) + numSlots);
		longAtput((void *)(newObj),((((((usqLong) (numSlotsMask()))) << (numSlotsFullShift()))) + ((((usqInt)(formatField) << (formatShift()))))) + ClassByteStringCompactIndex);
	}
	else {
		longAtput((void *)(newObj),((((((usqLong) numSlots)) << (numSlotsFullShift()))) + ((((usqInt)(formatField) << (formatShift()))))) + ClassByteStringCompactIndex);
	}

	/* for header parsing we put a saturated slot count in the prepended overflow size word */
	assert((numBytes % (allocationUnit())) == 0);
	assert((newObj % (allocationUnit())) == 0);
	freeStart += numBytes;
	newString = newObj;
	/* end allocateSlots:format:classIndex: */
l1:
	if (newString) {
		strncpy(((char *) (newString + BaseHeaderSize)), aCString, len);
	}
	return newString;
}