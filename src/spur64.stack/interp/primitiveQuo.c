/* Extracted from interp.c:21263 (function primitiveQuo). */

/*	Rounds negative results towards zero. */

	/* InterpreterPrimitives>>#primitiveQuo */

static void
primitiveQuo(void)
{
    sqInt integerArg;
    sqInt integerPointer;
    sqInt integerRcvr;
    sqInt integerResult;
    char *sp;

	integerResult = 0;

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

	/* begin success: */
	if (!integerArg) {
		if (!primFailCode) {
			primFailCode = 1;
		}
	}
	if (!primFailCode) {
		integerResult = /* quot:ient: */
				(integerRcvr > 0
					? (integerArg > 0
							? integerRcvr / integerArg
							: 0 - (integerRcvr / (0 - integerArg)))
					: (integerArg > 0
							? 0 - ((0 - integerRcvr) / integerArg)
							: (0 - integerRcvr) / (0 - integerArg)));
	}

	/* begin pop2AndPushIntegerIfOK: */
	if (!primFailCode) {
		if ((((((usqInt)(integerResult)) >> 60) + 1) & 15) <= 1) {
			/* begin pop:thenPush: */
			longAtput((sp = stackPointer + (1 * BytesPerWord)),(((usqInt)integerResult << 3) | 1));
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