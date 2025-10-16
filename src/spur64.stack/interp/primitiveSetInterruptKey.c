/* Extracted from interp.c:22203 (function primitiveSetInterruptKey). */

static void
primitiveSetInterruptKey(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerPointer;
    sqInt keycode;

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer));
	if ((((integerPointer) & 7) == 1)) {
		keycode = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		keycode = 0;
	}
	if (!GIV(primFailCode)) {
		/* begin setInterruptKeycode: */
		GIV(interruptKeycode) = keycode;
		GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
	}
}