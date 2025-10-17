/* Extracted from interp.c:11995 (function primitiveArrayBecomeOneWayNoCopyHash). */

/*	Similar to primitiveArrayBecomeOneWay but does /not/ copy the receiver's
	elements identity hashes over the argument's elements identity hashes.
	See the comment in primitiveArrayBecome for handling pervasive effects on
	method cacheing and jitting. */

	/* InterpreterPrimitives>>#primitiveArrayBecomeOneWayNoCopyHash */

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