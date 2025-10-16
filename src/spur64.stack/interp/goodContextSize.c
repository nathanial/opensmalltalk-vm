/* Extracted from interp.c:36031 (function goodContextSize). */

static NoDbgRegParms sqInt
goodContextSize(sqInt oop)
{
    usqInt numSlots;
    usqInt numSlotsUsqInt;

	/* begin numSlotsOf: */
	assert((classIndexOf(oop)) > (isForwardedObjectClassIndexPun()));
	numSlots = (((numSlotsUsqInt = byteAt((void *)(oop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(oop - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlotsUsqInt);
	return (numSlots == SmallContextSlots)
	 || (numSlots == LargeContextSlots);
}