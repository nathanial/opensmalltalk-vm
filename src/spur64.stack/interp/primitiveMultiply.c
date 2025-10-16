/* Extracted from interp.c:19327 (function primitiveMultiply). */

static void
primitiveMultiply(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerArg;
    sqInt integerPointer;
    sqInt integerRcvr;
    sqInt integerResult;
    sqInt overflow;
    char *sp;

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		integerRcvr = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		integerRcvr = 0;
	}

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer));
	if ((((integerPointer) & 7) == 1)) {
		integerArg = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		integerArg = 0;
	}
	if (!GIV(primFailCode)) {
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
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
		}
		else {
			integerResult = integerRcvr * integerArg;

			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),(((usqInt)integerResult << 3) | 1));
			GIV(stackPointer) = sp;
		}
	}
}