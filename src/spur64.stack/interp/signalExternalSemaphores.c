/* Extracted from interp.c:63371 (function signalExternalSemaphores). */

static sqInt
signalExternalSemaphores(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt numSlots;
    sqInt xArray;

	xArray = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ExternalObjectsArray) << (shiftForWord()))))));
	return doSignalExternalSemaphores(
		(/* begin numSlotsOf: */
			assert((classIndexOf(xArray)) > (isForwardedObjectClassIndexPun())),
		(((numSlots = byteAt((void *)(xArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(xArray - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots)));
}