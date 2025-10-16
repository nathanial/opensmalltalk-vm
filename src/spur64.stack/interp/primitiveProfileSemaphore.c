/* Extracted from interp.c:21182 (function primitiveProfileSemaphore). */

EXPORT(sqInt)
primitiveProfileSemaphore(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt sema;

	if (!(GIV(argumentCount) == 1)) {
		return (GIV(primFailCode) = PrimErrBadNumArgs);
	}
	sema = longAt(GIV(stackPointer));
	if (sema == GIV(nilObj)) {
		GIV(profileSemaphore) = null;

		/* begin zeroNextProfileTick */
		GIV(nextProfileTick) = 0;
	}
	else {
		if (!(/* isSemaphoreOop: */
				((!(sema & (tagMask()))))
			 && (((longAt((void *)(sema))) & (classIndexMask())) == (rawHashBitsOf(longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassSemaphore) << (shiftForWord()))))))))))) {
			return (GIV(primFailCode) = PrimErrBadArgument);
		}
		GIV(profileSemaphore) = sema;
	}
	GIV(profileProcess) = (GIV(profileMethod) = null);

	/* begin pop: */
	GIV(stackPointer) += 1 * BytesPerWord;
	return 0;
}