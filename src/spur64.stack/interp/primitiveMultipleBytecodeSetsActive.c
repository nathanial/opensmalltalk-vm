/* Extracted from interp.c:19296 (function primitiveMultipleBytecodeSetsActive). */

/*	Given one boolean parameter, set multipleBytecodeSetsActive to inform
	the VM that alternate bytecode sets such as SistaV1 are now in use and
	that the image format number should be updated accordingly. With zero
	parameters, answer the current value of multipleBytecodeSetsActive. */

	/* InterpreterPrimitives>>#primitiveMultipleBytecodeSetsActive */

EXPORT(sqInt)
primitiveMultipleBytecodeSetsActive(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	if (GIV(argumentCount) > 1) {
		return (GIV(primFailCode) = PrimErrBadNumArgs);
	}
	if (GIV(argumentCount) == 1) {
		if ((longAt(GIV(stackPointer))) == GIV(trueObj)) {
			GIV(multipleBytecodeSetsActive) = 1;
		}
		else {
			if (!((longAt(GIV(stackPointer))) == GIV(falseObj))) {
				return (GIV(primFailCode) = PrimErrBadArgument);
			}
			GIV(multipleBytecodeSetsActive) = 0;
		}
	}

	/* begin methodReturnBool: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),/* booleanObjectOf: */
		(GIV(multipleBytecodeSetsActive)
			? GIV(trueObj)
			: GIV(falseObj)));
	GIV(stackPointer) = sp;
	return 0;
}