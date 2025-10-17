/* Extracted from interp.c:68585 (function primitiveGetSetFFIExceptionHandling). */

	/* StackInterpreterPrimitives>>#primitiveGetSetFFIExceptionHandling */

static void
primitiveGetSetFFIExceptionHandling(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arg;
    sqInt integer;
    char *sp;

	if (!argumentCount) {
		integer = (ioCanCatchFFIExceptions()
					? ffiExceptionResponse
					: -1);

		/* begin methodReturnInteger: */
		assert(!((failed())));
		longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),(((usqInt)integer << 3) | 1));
		stackPointer = sp;
		return;
	}
	if (!(ioCanCatchFFIExceptions())) {
		/* primitiveFailFor: */
		primFailCode = PrimErrUnsupported;
		return;
	}
	if (!(argumentCount == 1)) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadNumArgs;
		return;
	}
	arg = longAt(stackPointer);
	if (!(((((arg) & 7) == 1))
		 && (((((arg = (arg >> 3))) >= -1) && (arg <= 1))))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}
	ffiExceptionResponse = arg;

	/* begin methodReturnReceiver */
	assert(!((failed())));
	stackPointer += argumentCount * BytesPerWord;
}