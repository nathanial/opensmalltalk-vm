/* Extracted from interp.c:21182 (function primitiveProfileSemaphore). */

/*	Primitive. Install the semaphore to be used for profiling, 
	or nil if no semaphore should be used.
	See also primitiveProfileStart. */

	/* InterpreterPrimitives>>#primitiveProfileSemaphore */

EXPORT(sqInt)
primitiveProfileSemaphore(void)
{
    sqInt sema;

	if (!(argumentCount == 1)) {
		return (primFailCode = PrimErrBadNumArgs);
	}
	sema = longAt(stackPointer);
	if (sema == nilObj) {
		profileSemaphore = null;

		/* begin zeroNextProfileTick */
		nextProfileTick = 0;
	}
	else {
		if (!(/* isSemaphoreOop: */
				((!(sema & (tagMask()))))
			 && (((longAt((void *)(sema))) & (classIndexMask())) == (rawHashBitsOf(longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassSemaphore) << (shiftForWord()))))))))))) {
			return (primFailCode = PrimErrBadArgument);
		}
		profileSemaphore = sema;
	}
	profileProcess = (profileMethod = null);

	/* begin pop: */
	stackPointer += 1 * BytesPerWord;
	return 0;
}