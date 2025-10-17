/* Extracted from interp.c:17963 (function primitiveInputSemaphore). */

/*	Register the input semaphore. If the argument is not a 
	Semaphore, unregister the current input semaphore. */

	/* InterpreterPrimitives>>#primitiveInputSemaphore */

static void
primitiveInputSemaphore(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arg;

	arg = longAt(GIV(stackPointer));
	if ((((arg) & 7) == 1)) {
		ioSetInputSemaphore((arg >> 3));
		if (!GIV(primFailCode)) {
			/* begin pop: */
			GIV(stackPointer) += 1 * BytesPerWord;
		}
		return;
	}

	/* If arg is integer, then use it as an index
	   into the external objects array and install it
	   as the new event semaphore
	   old code for compatibility */

	/* begin primitiveFail */
	if (!GIV(primFailCode)) {
		GIV(primFailCode) = 1;
	}
	return;
}