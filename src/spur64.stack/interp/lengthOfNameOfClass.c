/* Extracted from interp.c:55518 (function lengthOfNameOfClass). */

	/* StackInterpreter>>#lengthOfNameOfClass: */

static NoDbgRegParms sqInt
lengthOfNameOfClass(sqInt classOop)
{
    sqInt fmt;
    usqInt numSlots;
    usqInt numSlots1;
    usqInt numSlotsUsqInt;
    sqInt objOop;

	/* begin numSlotsOf: */
	assert((classIndexOf(classOop)) > (isForwardedObjectClassIndexPun()));
	numSlots = (((numSlotsUsqInt = byteAt((void *)(classOop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(classOop - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlotsUsqInt);
	if (numSlots == metaclassNumSlots) {
		return lengthOfNameOfClass(longAt((void *)((classOop + BaseHeaderSize) + ((((usqInt)(thisClassIndex) << (shiftForWord())))))));
	}
	if (numSlots <= classNameIndex) {
		return 0;
	}
	objOop = longAt((void *)((classOop + BaseHeaderSize) + ((((usqInt)(classNameIndex) << (shiftForWord()))))));

	/* begin lengthOf: */
	fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
	numSlotsUsqInt = byteAt((void *)(objOop + (numSlotsFieldByteOffset())));
	numSlots1 = (numSlotsUsqInt == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlotsUsqInt);
	if (fmt <= (ephemeronFormat())) {
		return numSlots1;
	}
	if (fmt >= (firstByteFormat())) {
		return ((numSlots1 << (shiftForWord()))) - (fmt & 7);
	}

	/* bytes, including CompiledMethod */
	if (fmt >= (firstShortFormat())) {
		return ((numSlots1 << ((shiftForWord()) - 1))) - (fmt & 3);
	}
	if (fmt >= (firstLongFormat())) {
		return ((numSlots1 << ((shiftForWord()) - 2))) - (fmt & 1);
	}
	if (fmt == (sixtyFourBitIndexableFormat())) {
		return numSlots1;
	}

	/* fmt = self forwardedFormat */
	return 0;
}