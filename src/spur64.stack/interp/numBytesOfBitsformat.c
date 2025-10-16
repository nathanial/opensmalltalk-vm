/* Extracted from interp.c:41044 (function numBytesOfBitsformat). */

static NoDbgRegParms sqInt
numBytesOfBitsformat(sqInt objOop, sqInt format)
{
    usqInt numBytes;
    usqInt numSlots;

	assert((isPureBitsFormat(format))
	 && ((formatOf(objOop)) == format));

	/* begin numSlotsOf: */
	assert((classIndexOf(objOop)) > (isForwardedObjectClassIndexPun()));
	numBytes = (((numSlots = byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	numBytes = (numBytes << (shiftForWord()));
	if (format < (firstShortFormat())) {
		return (format <= (sixtyFourBitIndexableFormat())
				? numBytes
				: numBytes - (((format & 1) << 2)));
	}
	return (format < (firstByteFormat())
			? numBytes - (((format & 3) << 1))
			: numBytes - (format & 7));
}