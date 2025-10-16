/* Extracted from interp.c:26424 (function addressAfter). */

static NoDbgRegParms usqInt
addressAfter(sqInt objOop)
{
    usqInt numSlots;

	numSlots = byteAt((void *)(objOop + (numSlotsFieldByteOffset())));
	if (!numSlots) {
		return (objOop + 8 /* allocationUnit */) + BaseHeaderSize;
	}
	if (numSlots == (numSlotsMask())) {
		numSlots = ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8;
	}
	return (objOop + BaseHeaderSize) + (((numSlots << (shiftForWord()))));
}