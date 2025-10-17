/* Extracted from interp.c:11921 (function primitiveArrayBecomeOneWay). */

/*	Invoke the one-way become primitive.
	See the comment in primitiveArrayBecome for handling pervasive effects on
	method cacheing and jitting. */

	/* InterpreterPrimitives>>#primitiveArrayBecomeOneWay */

static void
primitiveArrayBecomeOneWay(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arg;
    sqInt ec;
    sqInt rcvr;

	arg = longAt(stackPointer);
	rcvr = longAt(stackPointer + (1 * BytesPerWord));
	ec = becomewithtwoWaycopyHash(rcvr, arg, 0, 1);

	/* begin primitiveBecomeReturn: */
	if (ec) {
		/* primitiveFailFor: */
		primFailCode = ec;
	}
	else {
		/* begin methodReturnReceiver */
		assert(!((failed())));
		stackPointer += argumentCount * BytesPerWord;
	}
}