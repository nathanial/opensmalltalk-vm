/* Extracted from interp.c:51953 (function doPrimitiveDivby). */

static NoDbgRegParms sqInt
doPrimitiveDivby(sqInt rcvr, sqInt arg)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerArg;
    sqInt integerRcvr;
    sqInt posArg;
    sqInt posRcvr;
    sqInt result;

	integerArg = 0;
	integerRcvr = 0;
	if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
		integerRcvr = (rcvr >> 3);
		integerArg = (arg >> 3);

		/* begin success: */
		if (!integerArg) {
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
		}
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
	}
	if (GIV(primFailCode)) {
		return 1;
	}
	if (integerRcvr > 0) {
		if (integerArg > 0) {
			result = integerRcvr / integerArg;
		}
		else {
			posArg = 0 - integerArg;
			result = 0 - ((integerRcvr + (posArg - 1)) / posArg);
		}
	}
	else {
		posRcvr = 0 - integerRcvr;
		if (integerArg > 0) {
			result = 0 - ((posRcvr + (integerArg - 1)) / integerArg);
		}
		else {
			posArg = 0 - integerArg;
			result = posRcvr / posArg;
		}
	}

	/* begin success: */
	if (!((((((usqInt)(result)) >> 60) + 1) & 15) <= 1)) {
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
	}
	return result;
}