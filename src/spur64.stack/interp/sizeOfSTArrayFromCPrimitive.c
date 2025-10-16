/* Extracted from interp.c:63516 (function sizeOfSTArrayFromCPrimitive). */

sqInt
sizeOfSTArrayFromCPrimitive(void *cPtr)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt fmt;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    sqInt oop;

	oop = (oopForPointer(cPtr)) - BaseHeaderSize;
	if (!(/* isWordsOrBytes: */
			((!(oop & (tagMask()))))
		 && (isWordsOrBytesNonImm(oop)))) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
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