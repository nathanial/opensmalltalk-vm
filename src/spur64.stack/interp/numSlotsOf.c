/* Extracted from interp.c:41237 (function numSlotsOf). */

/*	numSlotsOf: should not be applied to free or forwarded objects. */

	/* SpurMemoryManager>>#numSlotsOf: */

usqInt
numSlotsOf(sqInt objOop)
{
    usqInt numSlots;

	assert((classIndexOf(objOop)) > (isForwardedObjectClassIndexPun()));
	return (((numSlots = byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots);
}