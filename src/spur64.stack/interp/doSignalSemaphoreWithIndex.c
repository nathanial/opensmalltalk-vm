/* Extracted from interp.c:52072 (function doSignalSemaphoreWithIndex). */

sqInt
doSignalSemaphoreWithIndex(sqInt index)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt sema;
    sqInt xArray;

	xArray = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ExternalObjectsArray) << (shiftForWord()))))));
	assert(isArray(xArray));

	/* Note: semaphore indices are 1-based */
	sema = longAt((void *)((xArray + BaseHeaderSize) + ((((usqInt)((index - 1)) << (shiftForWord()))))));
	assert(!(isOopForwarded(sema)));
	return (/* isSemaphoreOop: */
		((!(sema & (tagMask()))))
	 && (((longAt((void *)(sema))) & (classIndexMask())) == (rawHashBitsOf(longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassSemaphore) << (shiftForWord()))))))))))
	 && (synchronousSignal(sema));
}