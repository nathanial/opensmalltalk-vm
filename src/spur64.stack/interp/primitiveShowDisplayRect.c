/* Extracted from interp.c:22488 (function primitiveShowDisplayRect). */

/*	Force the given rectangular section of the Display to be 
	copied to the screen. */

	/* InterpreterPrimitives>>#primitiveShowDisplayRect */

static void
primitiveShowDisplayRect(void)
{
    sqInt bottom;
    sqInt integerPointer;
    sqInt left;
    sqInt right;
    sqInt top;

	/* begin stackIntegerValue: */
	integerPointer = longAt(stackPointer);
	if ((((integerPointer) & 7) == 1)) {
		bottom = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		bottom = 0;
	}

	/* begin stackIntegerValue: */
	integerPointer = longAt(stackPointer + (1 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		top = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		top = 0;
	}

	/* begin stackIntegerValue: */
	integerPointer = longAt(stackPointer + (2 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		right = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		right = 0;
	}

	/* begin stackIntegerValue: */
	integerPointer = longAt(stackPointer + (3 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		left = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		left = 0;
	}
	if (!primFailCode) {
		updateDisplayLeftTopRightBottom(left, top, right, bottom);
		ioForceDisplayUpdate();

		/* begin methodReturnReceiver */
		assert(!((failed())));
		stackPointer += argumentCount * BytesPerWord;
	}
}