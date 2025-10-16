/* Extracted from interp.c:41215 (function numSlotsOfIndexablePointerObj). */

static NoDbgRegParms usqInt
numSlotsOfIndexablePointerObj(sqInt objOop)
{
    usqInt numSlots;

	assert((formatOf(objOop)) == (indexablePointersFormat()));
	if (((longAt((void *)(objOop))) & (classIndexMask())) == ClassMethodContextCompactIndex) {
		return CtxtTempFrameStart + (fetchStackPointerOf(objOop));
	}
	else {
		/* begin numSlotsOf: */
		assert((classIndexOf(objOop)) > (isForwardedObjectClassIndexPun()));
		return (((numSlots = byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	}
}