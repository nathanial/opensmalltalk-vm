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

	if (argumentCount > 1) {
		return (primFailCode = PrimErrBadNumArgs);
	}
	if (argumentCount == 1) {
		if ((longAt(stackPointer)) == trueObj) {
			multipleBytecodeSetsActive = 1;
		}
		else {
			if (!((longAt(stackPointer)) == falseObj)) {
				return (primFailCode = PrimErrBadArgument);
			}
			multipleBytecodeSetsActive = 0;
		}
	}

	/* begin methodReturnBool: */
	assert(!((failed())));
	longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),/* booleanObjectOf: */
		(multipleBytecodeSetsActive
			? trueObj
			: falseObj));
	stackPointer = sp;
	return 0;
}