/* Extracted from interp.c:22203 (function primitiveSetInterruptKey). */

 /* IMMUTABILITY */


/*	Set the user interrupt keycode. The keycode is an integer whose encoding
	is described in the comment for primitiveKbdNext.
 */

	/* InterpreterPrimitives>>#primitiveSetInterruptKey */

static void
primitiveSetInterruptKey(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerPointer;
    sqInt keycode;

	/* begin stackIntegerValue: */
	integerPointer = longAt(stackPointer);
	if ((((integerPointer) & 7) == 1)) {
		keycode = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		keycode = 0;
	}
	if (!primFailCode) {
		/* begin setInterruptKeycode: */
		interruptKeycode = keycode;
		stackPointer += argumentCount * BytesPerWord;
	}
}