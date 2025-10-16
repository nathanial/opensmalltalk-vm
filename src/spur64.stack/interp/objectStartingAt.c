/* Extracted from interp.c:41601 (function objectStartingAt). */

static NoDbgRegParms sqInt
objectStartingAt(sqInt address)
{
    usqInt numSlots;

	numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
	return (numSlots == (numSlotsMask())
			? address + BaseHeaderSize
			: address);
}