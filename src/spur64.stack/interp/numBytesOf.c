/* Extracted from interp.c:41095 (function numBytesOf). */

sqInt
numBytesOf(sqInt objOop)
{
    sqInt fmt;
    usqInt numBytes;
    usqInt numSlots;

	fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());

	/* begin numSlotsOf: */
	assert((classIndexOf(objOop)) > (isForwardedObjectClassIndexPun()));
	numBytes = (((numSlots = byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	numBytes = (numBytes << (shiftForWord()));
	if (fmt >= (firstByteFormat())) {
		return numBytes - (fmt & 7);
	}

	/* bytes (the common case), including CompiledMethod */
	if (fmt <= (sixtyFourBitIndexableFormat())) {
		return numBytes;
	}
	if (fmt >= (firstShortFormat())) {
		return numBytes - (((fmt & 3) << 1));
	}

	/* fmt >= self firstLongFormat */
	return numBytes - (((fmt & 1) << 2));
}