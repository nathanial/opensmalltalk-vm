/* Extracted from interp.c:27423 (function lengthOfformat). */

/*	Answer the number of indexable units in the given object.
	For a CompiledMethod, the size of the method header (in bytes)
	should be subtracted from the result of this method. */

	/* Spur64BitMemoryManager>>#lengthOf:format: */

static NoDbgRegParms sqInt
lengthOfformat(sqInt objOop, sqInt fmt)
{
    usqInt numSlots;
    usqInt numSlotsUsqInt;

	/* begin numSlotsOfAny: */
	numSlotsUsqInt = byteAt((void *)(objOop + (numSlotsFieldByteOffset())));
	numSlots = (numSlotsUsqInt == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8
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