/* Extracted from interp.c:22488 (function primitiveShowDisplayRect). */

static void
primitiveShowDisplayRect(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt bottom;
    sqInt integerPointer;
    sqInt left;
    sqInt right;
    sqInt top;

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer));
	if ((((integerPointer) & 7) == 1)) {
		bottom = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		bottom = 0;
	}

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		top = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		top = 0;
	}

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer) + (2 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		right = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		right = 0;
	}

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer) + (3 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		left = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		left = 0;
	}
	if (!GIV(primFailCode)) {
		updateDisplayLeftTopRightBottom(left, top, right, bottom);
		ioForceDisplayUpdate();

		/* begin methodReturnReceiver */
		assert(!((failed())));
		GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
	}
}