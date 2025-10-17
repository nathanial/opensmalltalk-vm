/* Extracted from interp.c:19932 (function primitiveObjectAt). */

/*	Defined for CompiledMethods only */

	/* InterpreterPrimitives>>#primitiveObjectAt */

static void
primitiveObjectAt(void)
{
    sqInt index;
    sqInt integerPointer;
    char *sp;
    sqInt thisReceiver;

	/* begin stackIntegerValue: */
	integerPointer = longAt(stackPointer);
	if ((((integerPointer) & 7) == 1)) {
		index = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		index = 0;
	}
	if (primFailCode) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}
	thisReceiver = longAt(stackPointer + (1 * BytesPerWord));
	if (!((index > 0)
		 && (index <= ((literalCountOf(thisReceiver)) + LiteralStart)))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadIndex;
		return;
	}

	/* begin pop:thenPush: */
	longAtput((sp = stackPointer + (1 * BytesPerWord)),longAt((void *)((thisReceiver + BaseHeaderSize) + ((((usqInt)((index - 1)) << (shiftForWord())))))));
	stackPointer = sp;
}