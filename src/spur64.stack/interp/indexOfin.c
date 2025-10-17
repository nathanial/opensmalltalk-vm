/* Extracted from interp.c:36260 (function indexOfin). */

 /* IMMUTABILITY */

	/* SpurMemoryManager>>#indexOf:in: */

sqInt
indexOfin(sqInt anElement, sqInt anObject)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt fmt;
    sqInt fmtSqInt;
    sqInt i;
    usqInt numSlots;
    usqInt numSlotsUsqInt;

	fmt = (byteAt((void *)(anObject + (formatFieldByteOffset())))) & (formatMask());
	if (fmt <= 5 /* lastPointerFormat */) {
		/* begin numSlotsOf: */
		assert((classIndexOf(anObject)) > (isForwardedObjectClassIndexPun()));
		numSlots = (((numSlotsUsqInt = byteAt((void *)(anObject + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(anObject - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlotsUsqInt);
		for (i = 0; i <= numSlots; i += 1) {
			if (anElement == (longAt((void *)((anObject + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord())))))))) {
				return i;
			}
		}
	}
	if (fmt >= (firstByteFormat())) {
		if (fmt >= (firstCompiledMethodFormat())) {
			return (primFailCode = PrimErrUnsupported);
		}

		/* begin numBytesOfBytes: */
		fmtSqInt = (byteAt((void *)(anObject + (formatFieldByteOffset())))) & (formatMask());
		assert(fmtSqInt >= (firstByteFormat()));
		numSlots = ((((/* begin numSlotsOf: */
	assert((classIndexOf(anObject)) > (isForwardedObjectClassIndexPun())),
(((numSlotsUsqInt = byteAt((void *)(anObject + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(anObject - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlotsUsqInt))) << (shiftForWord()))) - (fmtSqInt & 7);
		for (i = 0; i <= numSlots; i += 1) {
			if (anElement == (byteAt((void *)((anObject + BaseHeaderSize) + i)))) {
				return i;
			}
		}
	}
	if (fmt >= (firstShortFormat())) {
		numSlots = ((usqInt)((numBytesOf(anObject)))) >> 1;
		for (i = 0; i <= numSlots; i += 1) {
			if (anElement == (((unsigned short) (shortAt((void *)((anObject + BaseHeaderSize) + ((((usqInt)(i) << 1))))))))) {
				return i;
			}
		}
	}
	if (fmt == (sixtyFourBitIndexableFormat())) {
		numSlots = ((usqInt)((numBytesOf(anObject)))) >> 3;
		for (i = 0; i <= numSlots; i += 1) {
			if (anElement == (long64At((void *)((anObject + BaseHeaderSize) + ((((usqInt)(i) << 3))))))) {
				return i;
			}
		}
	}
	if (fmt >= (firstLongFormat())) {
		numSlots = ((usqInt)((numBytesOf(anObject)))) >> 2;
		for (i = 0; i <= numSlots; i += 1) {
			if (anElement == (long32At((void *)((anObject + BaseHeaderSize) + ((((usqInt)(i) << 2))))))) {
				return i;
			}
		}
	}
	return -1;
}