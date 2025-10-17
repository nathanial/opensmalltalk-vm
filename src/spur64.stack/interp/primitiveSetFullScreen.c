/* Extracted from interp.c:22022 (function primitiveSetFullScreen). */

/*	On platforms that support it, set full-screen mode to the value of the
	boolean argument.
 */

	/* InterpreterPrimitives>>#primitiveSetFullScreen */

static void
primitiveSetFullScreen(void)
{
    sqInt argOop;

	argOop = longAt(stackPointer);
	if (argOop == trueObj) {
		ioSetFullScreen(1);
	}
	else {
		if (argOop == falseObj) {
			ioSetFullScreen(0);
		}
		else {
			/* begin primitiveFail */
			if (!primFailCode) {
				primFailCode = 1;
			}
		}
	}
	if (!primFailCode) {
		/* begin setFullScreenFlag: */
		fullScreenFlag = argOop == trueObj;
		assert(!((failed())));
		stackPointer += argumentCount * BytesPerWord;
	}
}