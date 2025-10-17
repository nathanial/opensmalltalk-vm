/* Extracted from interp.c:14795 (function primitiveDivide). */

	/* InterpreterPrimitives>>#primitiveDivide */

static void
primitiveDivide(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerArgument;
    sqInt integerPointer;
    sqInt integerReceiver;
    char *sp;

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		integerReceiver = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		integerReceiver = 0;
	}

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer));
	if ((((integerPointer) & 7) == 1)) {
		integerArgument = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		integerArgument = 0;
	}
	if ((integerArgument != 0)
	 && ((integerReceiver % integerArgument) == 0)) {
		/* begin pop2AndPushIntegerIfOK: */
		if (!GIV(primFailCode)) {
			if ((((((usqInt)((integerReceiver / integerArgument))) >> 60) + 1) & 15) <= 1) {
				/* begin pop:thenPush: */
				longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),(((usqInt)(integerReceiver / integerArgument) << 3) | 1));
				GIV(stackPointer) = sp;
			}
			else {
				/* begin success: */
				if (!GIV(primFailCode)) {
					GIV(primFailCode) = 1;
				}
			}
		}
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
	}
}