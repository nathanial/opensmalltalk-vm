/* Extracted from interp.c:14795 (function primitiveDivide). */

	/* InterpreterPrimitives>>#primitiveDivide */

static void
primitiveDivide(void)
{
    sqInt integerArgument;
    sqInt integerPointer;
    sqInt integerReceiver;
    char *sp;

	/* begin stackIntegerValue: */
	integerPointer = longAt(stackPointer + (1 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		integerReceiver = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		integerReceiver = 0;
	}

	/* begin stackIntegerValue: */
	integerPointer = longAt(stackPointer);
	if ((((integerPointer) & 7) == 1)) {
		integerArgument = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		integerArgument = 0;
	}
	if ((integerArgument != 0)
	 && ((integerReceiver % integerArgument) == 0)) {
		/* begin pop2AndPushIntegerIfOK: */
		if (!primFailCode) {
			if ((((((usqInt)((integerReceiver / integerArgument))) >> 60) + 1) & 15) <= 1) {
				/* begin pop:thenPush: */
				longAtput((sp = stackPointer + (1 * BytesPerWord)),(((usqInt)(integerReceiver / integerArgument) << 3) | 1));
				stackPointer = sp;
			}
			else {
				/* begin success: */
				if (!primFailCode) {
					primFailCode = 1;
				}
			}
		}
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
	}
}