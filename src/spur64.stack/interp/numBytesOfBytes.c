/* Extracted from interp.c:41075 (function numBytesOfBytes). */

/*	Answer the number of indexable bytes in the given non-immediate
	byte-indexable object.
 */

	/* SpurMemoryManager>>#numBytesOfBytes: */

static NoDbgRegParms sqInt
numBytesOfBytes(sqInt objOop)
{
    sqInt fmt;
    usqInt numSlots;

	fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
	assert(fmt >= (firstByteFormat()));
	return ((((/* begin numSlotsOf: */
	assert((classIndexOf(objOop)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) << (shiftForWord()))) - (fmt & 7);
}