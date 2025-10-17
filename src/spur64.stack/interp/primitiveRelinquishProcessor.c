/* Extracted from interp.c:21380 (function primitiveRelinquishProcessor). */

/*	Relinquish the processor for up to the given number of microseconds. The
	exact behavior of this primitive is platform dependent.
 */

	/* InterpreterPrimitives>>#primitiveRelinquishProcessor */

static void
primitiveRelinquishProcessor(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerPointer;
    sqInt microSecs;

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer));
	if ((((integerPointer) & 7) == 1)) {
		microSecs = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		microSecs = 0;
	}
	if (!GIV(primFailCode)) {
		if (!GIV(nextProfileTick)) {
			ioRelinquishProcessorForMicroseconds(microSecs);

			/* In simulation we allow ioRelinquishProcessorForMicroseconds: to fail so that
			   we can arrange that the simulator responds to input events promply.  This
			   *DOES NOT HAPPEN* in the real vm. */
		}

		/* begin pop: */
		GIV(stackPointer) += 1 * BytesPerWord;
	}
}