/* Extracted from interp.c:38124 (function lengthOfMaybeImmediate). */

static NoDbgRegParms sqInt
lengthOfMaybeImmediate(sqInt oop)
{
    sqInt fmt;
    usqInt numSlots;
    usqInt numSlotsUsqInt;

	if (((oop & (tagMask())) != 0)) {
		return 0;
	}

	/* begin lengthOf: */
	fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
	numSlotsUsqInt = byteAt((void *)(oop + (numSlotsFieldByteOffset())));
	numSlots = (numSlotsUsqInt == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(oop - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlotsUsqInt);
	if (fmt <= (ephemeronFormat())) {
		return numSlots;
	}
	if (fmt >= (firstByteFormat())) {
		return ((numSlots << (shiftForWord()))) - (fmt & 7);
	}

	/* bytes, including CompiledMethod */
	if (fmt >= (firstShortFormat())) {
		return ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
	}
	if (fmt >= (firstLongFormat())) {
		return ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
	}
	if (fmt == (sixtyFourBitIndexableFormat())) {
		return numSlots;
	}

	/* fmt = self forwardedFormat */
	return 0;
}