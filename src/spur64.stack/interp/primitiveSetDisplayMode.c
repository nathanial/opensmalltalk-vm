/* Extracted from interp.c:21928 (function primitiveSetDisplayMode). */

static void
primitiveSetDisplayMode(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt d;
    sqInt fsFlag;
    sqInt h;
    sqInt integerPointer;
    sqInt okay;
    char *sp;
    sqInt w;

	/* begin booleanValueOf: */
	if ((longAt(GIV(stackPointer))) == GIV(trueObj)) {
		fsFlag = 1;
		goto l1;
	}
	if ((longAt(GIV(stackPointer))) == GIV(falseObj)) {
		fsFlag = 0;
		goto l1;
	}

	/* begin success: */
	if (!GIV(primFailCode)) {
		GIV(primFailCode) = 1;
	}
	fsFlag = null;
	/* end booleanValueOf: */
l1:

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		h = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		h = 0;
	}

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer) + (2 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		w = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		w = 0;
	}

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer) + (3 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		d = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		d = 0;
	}
	if (GIV(primFailCode)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	okay = ioSetDisplayMode(w, h, d, fsFlag);
	if (GIV(primFailCode)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrOperationFailed;
		return;
	}

	/* begin methodReturnBool: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),/* booleanObjectOf: */
		(okay
			? GIV(trueObj)
			: GIV(falseObj)));
	GIV(stackPointer) = sp;
}