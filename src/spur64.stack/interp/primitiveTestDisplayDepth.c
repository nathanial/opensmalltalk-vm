/* Extracted from interp.c:25212 (function primitiveTestDisplayDepth). */

/*	Return true if the host OS does support the given display depth. */

	/* InterpreterPrimitives>>#primitiveTestDisplayDepth */

static void
primitiveTestDisplayDepth(void)
{
    sqInt bitsPerPixel;
    sqInt integerPointer;
    sqInt okay;
    char *sp;

	okay = 0;

	/* begin stackIntegerValue: */
	integerPointer = longAt(stackPointer);
	if ((((integerPointer) & 7) == 1)) {
		bitsPerPixel = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		bitsPerPixel = 0;
	}
	if (!primFailCode) {
		okay = ioHasDisplayDepth(bitsPerPixel);
	}
	if (!primFailCode) {
		/* begin pop:thenPushBool: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),/* booleanObjectOf: */
			(okay
				? trueObj
				: falseObj));
		stackPointer = sp;
	}
}