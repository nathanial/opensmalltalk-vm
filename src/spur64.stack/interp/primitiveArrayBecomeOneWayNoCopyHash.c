/* Extracted from interp.c:11995 (function primitiveArrayBecomeOneWayNoCopyHash). */

static void
primitiveArrayBecomeOneWayNoCopyHash(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arg;
    sqInt ec;
    sqInt rcvr;

	arg = longAt(GIV(stackPointer));
	rcvr = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	ec = becomewithtwoWaycopyHash(rcvr, arg, 0, 0);

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