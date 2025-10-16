/* Extracted from interp.c:27094 (function initFreeChunkWithBytesat). */

static NoDbgRegParms sqInt
initFreeChunkWithBytesat(usqLong numBytes, sqInt address)
{
    sqLong numSlots;

	assert(((numBytes % (allocationUnit())) == 0)
	 && (numBytes >= (BaseHeaderSize + BytesPerOop)));

	/* double header */
	if (numBytes >= (((((usqInt)((numSlotsMask())) << (shiftForWord())))) + BaseHeaderSize)) {
		numSlots = ((usqLong)(((numBytes - BaseHeaderSize) - BaseHeaderSize))) >> (shiftForWord());
		longAtput((void *)(address),((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) + numSlots);
		longAtput((void *)(address + 8),((sqInt)((usqInt)((numSlotsMask())) << (numSlotsFullShift()))));
		return address + 8;
	}

	/* single header */
	numSlots = ((usqLong)((numBytes - BaseHeaderSize))) >> (shiftForWord());
	assert(numSlots < (numSlotsMask()));
	longAtput((void *)(address),((sqLong)((usqLong)(numSlots) << (numSlotsFullShift()))));
	return address;
}