/* Extracted from interp.c:13367 (function primitiveChangeClass). */

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