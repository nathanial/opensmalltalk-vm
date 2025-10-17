/* Extracted from interp.c:68585 (function primitiveGetSetFFIExceptionHandling). */

	/* StackInterpreterPrimitives>>#primitiveGetSetFFIExceptionHandling */

static void
primitiveGetSetFFIExceptionHandling(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arg;
    sqInt integer;
    char *sp;

	if (!GIV(argumentCount)) {
		integer = (ioCanCatchFFIExceptions()
					? ffiExceptionResponse
					: -1);

		/* begin methodReturnInteger: */
		assert(!((failed())));
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)integer << 3) | 1));
		GIV(stackPointer) = sp;
		return;
	}
	if (!(ioCanCatchFFIExceptions())) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrUnsupported;
		return;
	}
	if (!(GIV(argumentCount) == 1)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadNumArgs;
		return;
	}
	arg = longAt(GIV(stackPointer));
	if (!(((((arg) & 7) == 1))
		 && (((((arg = (arg >> 3))) >= -1) && (arg <= 1))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	ffiExceptionResponse = arg;

	/* begin methodReturnReceiver */
	assert(!((failed())));
	GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
}