/* Extracted from interp.c:49102 (function bridgeAt). */

static NoDbgRegParms sqInt
bridgeAt(sqInt segIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    usqInt numSlots;

	/* begin bridgeFor: */
	address = (((((&(GIV(segments)[segIndex])))->segSize)) + ((((&(GIV(segments)[segIndex])))->segStart))) - (2 * BaseHeaderSize);
	numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
	return (numSlots == (numSlotsMask())
			? address + BaseHeaderSize
			: address);
}