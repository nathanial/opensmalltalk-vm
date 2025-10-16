/* Extracted from interp.c:11954 (function primitiveArrayBecomeOneWayCopyHashArg). */

static void
primitiveArrayBecomeOneWayCopyHashArg(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt copyHashFlag;
    sqInt ec;

	copyHashFlag = 0;
	if ((longAt(GIV(stackPointer))) == GIV(trueObj)) {
		copyHashFlag = 1;
	}
	else {
		if ((longAt(GIV(stackPointer))) == GIV(falseObj)) {
			copyHashFlag = 0;
		}
		else {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadArgument;
			return;
		}
	}
	ec = becomewithtwoWaycopyHash(longAt(GIV(stackPointer) + (2 * BytesPerWord)), longAt(GIV(stackPointer) + (1 * BytesPerWord)), 0, copyHashFlag);

	/* begin primitiveBecomeReturn: */
	if (ec) {
		/* primitiveFailFor: */
		GIV(primFailCode) = ec;
	}
	else {
		/* begin methodReturnReceiver */
		assert(!((failed())));
		GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
	}
}