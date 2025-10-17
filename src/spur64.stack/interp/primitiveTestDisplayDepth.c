/* Extracted from interp.c:25212 (function primitiveTestDisplayDepth). */

/*	Return true if the host OS does support the given display depth. */

	/* InterpreterPrimitives>>#primitiveTestDisplayDepth */

static void
primitiveTestDisplayDepth(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt bitsPerPixel;
    sqInt integerPointer;
    sqInt okay;
    char *sp;

	okay = 0;

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer));
	if ((((integerPointer) & 7) == 1)) {
		bitsPerPixel = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		bitsPerPixel = 0;
	}
	if (!GIV(primFailCode)) {
		okay = ioHasDisplayDepth(bitsPerPixel);
	}
	if (!GIV(primFailCode)) {
		/* begin pop:thenPushBool: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),/* booleanObjectOf: */
			(okay
				? GIV(trueObj)
				: GIV(falseObj)));
		GIV(stackPointer) = sp;
	}
}