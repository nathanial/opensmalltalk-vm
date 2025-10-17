/* Extracted from interp.c:19327 (function primitiveMultiply). */

	/* InterpreterPrimitives>>#primitiveMultiply */

static void
primitiveMultiply(void)
{
    sqInt integerArg;
    sqInt integerPointer;
    sqInt integerRcvr;
    sqInt integerResult;
    sqInt overflow;
    char *sp;

	/* begin stackIntegerValue: */
	integerPointer = longAt(stackPointer + (1 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		integerRcvr = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		integerRcvr = 0;
	}

	/* begin stackIntegerValue: */
	integerPointer = longAt(stackPointer);
	if ((((integerPointer) & 7) == 1)) {
		integerArg = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		integerArg = 0;
	}
	if (!primFailCode) {
		overflow = (integerRcvr > 0
					? (integerArg > 0
							? integerRcvr > ((MaxSmallInteger) / integerArg)
							: integerArg < ((MinSmallInteger) / integerRcvr))
					: (integerArg > 0
							? integerRcvr < ((MinSmallInteger) / integerArg)
							: (integerRcvr < 0)
							 && (integerArg < ((MaxSmallInteger) / integerRcvr))));
		if (overflow) {
			/* begin primitiveFail */
			if (!primFailCode) {
				primFailCode = 1;
			}
		}
		else {
			integerResult = integerRcvr * integerArg;

			/* begin pop:thenPush: */
			longAtput((sp = stackPointer + (1 * BytesPerWord)),(((usqInt)integerResult << 3) | 1));
			stackPointer = sp;
		}
	}
}