/* Extracted from interp.c:52072 (function doSignalSemaphoreWithIndex). */

/*	Signal the external semaphore with the given index. Answer if a context
	switch occurs as a result. Do not bounds check. This has been done in the
	caller.  */

	/* StackInterpreter>>#doSignalSemaphoreWithIndex: */

sqInt
doSignalSemaphoreWithIndex(sqInt index)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt sema;
    sqInt xArray;

	xArray = longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ExternalObjectsArray) << (shiftForWord()))))));
	assert(isArray(xArray));

	/* Note: semaphore indices are 1-based */
	sema = longAt((void *)((xArray + BaseHeaderSize) + ((((usqInt)((index - 1)) << (shiftForWord()))))));
	assert(!(isOopForwarded(sema)));
	return (/* isSemaphoreOop: */
		((!(sema & (tagMask()))))
	 && (((longAt((void *)(sema))) & (classIndexMask())) == (rawHashBitsOf(longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassSemaphore) << (shiftForWord()))))))))))
	 && (synchronousSignal(sema));
}