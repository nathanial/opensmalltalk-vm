/* Extracted from interp.c:41198 (function numSlotsOfAny). */

/*	A private internal version of numSlotsOf: that is happy to be applied to
	free or forwarded objects.
 */

	/* SpurMemoryManager>>#numSlotsOfAny: */

static NoDbgRegParms usqInt
numSlotsOfAny(sqInt objOop)
{
    usqInt numSlots;

	numSlots = byteAt((void *)(objOop + (numSlotsFieldByteOffset())));
	return (numSlots == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots);
}