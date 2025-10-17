/* Extracted from interp.c:63371 (function signalExternalSemaphores). */

/*	Signal all requested semaphores. Answer if a context switch has occurred. */

	/* StackInterpreter>>#signalExternalSemaphores */

static sqInt
signalExternalSemaphores(void)
{
    usqInt numSlots;
    sqInt xArray;

	xArray = longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ExternalObjectsArray) << (shiftForWord()))))));
	return doSignalExternalSemaphores(
		(/* begin numSlotsOf: */
			assert((classIndexOf(xArray)) > (isForwardedObjectClassIndexPun())),
		(((numSlots = byteAt((void *)(xArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(xArray - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots)));
}