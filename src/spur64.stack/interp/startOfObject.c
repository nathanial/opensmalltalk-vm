/* Extracted from interp.c:45693 (function startOfObject). */

static NoDbgRegParms usqInt
startOfObject(sqInt objOop)
{
	return ((byteAt((void *)(objOop + (numSlotsFieldByteOffset())))) == (numSlotsMask())
			? objOop - BaseHeaderSize
			: objOop);
}