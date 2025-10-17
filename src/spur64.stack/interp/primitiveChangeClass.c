/* Extracted from interp.c:13367 (function primitiveChangeClass). */

/*	Primitive. Change the class of the receiver into the class of the argument
	given that
	the format of the receiver matches the format of the argument's class.
	Fail if the
	receiver or argument are SmallIntegers, or the receiver is an instance of
	a compact
	class and the argument isn't, or when the argument's class is compact and
	the receiver
	isn't, or when the format of the receiver is different from the format of
	the argument's
	class, or when the arguments class is fixed and the receiver's size
	differs from the size
	that an instance of the argument's class should have. */

	/* InterpreterPrimitives>>#primitiveChangeClass */

static void
primitiveChangeClass(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arg;
    sqInt argClass;
    sqInt err;
    sqInt oop;
    sqInt rcvr;

	/* begin stackObjectValue: */
	oop = longAt(GIV(stackPointer));
	if (((oop & (tagMask())) != 0)) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		arg = null;
		goto l1;
	}
	arg = oop;
	/* end stackObjectValue: */
l1:

	/* begin stackObjectValue: */
	oop = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if (((oop & (tagMask())) != 0)) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		rcvr = null;
		goto l2;
	}
	rcvr = oop;
	/* end stackObjectValue: */
l2:
	if (GIV(primFailCode)) {
		return;
	}
	argClass = fetchClassOfNonImm(arg);
	err = changeClassOfto(rcvr, argClass);
	if (err) {
		/* primitiveFailFor: */
		GIV(primFailCode) = err;
	}
	else {
		/* begin flushAtCache */
		memset(GIV(atCache), 0, AtCacheTotalSize * (sizeof(GIV(atCache)[0])));
		GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
	}

	/* Flush at cache because rcvr's class has changed. */
	return;
}