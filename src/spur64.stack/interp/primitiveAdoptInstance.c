/* Extracted from interp.c:11709 (function primitiveAdoptInstance). */

static void
primitiveAdoptInstance(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arg;
    sqInt err;
    sqInt rcvr;

	arg = longAt(GIV(stackPointer));
	rcvr = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if ((((arg & (tagMask())) != 0))
	 || ((GIV(argumentCount) > 1)
	 && ((((rcvr & (tagMask())) != 0))
	 || (!(objCouldBeClassObj(rcvr)))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	err = changeClassOfto(arg, rcvr);
	if (err) {
		if (err == PrimErrBadReceiver) {
			err = PrimErrBadArgument;
		}
		else {
			if (err == PrimErrBadArgument) {
				err = PrimErrBadReceiver;
			}
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = err;
	}
	else {
		/* begin flushAtCache */
		memset(GIV(atCache), 0, AtCacheTotalSize * (sizeof(GIV(atCache)[0])));
		GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
	}

	/* Flush at cache because rcvr's class has changed.
	   changeClassOf:to: answers errors as if rcvr (the class) is an argument... */
	return;
}