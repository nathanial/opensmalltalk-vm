/* Extracted from interp.c:52977 (function fetchIntegerofObject). */

/*	Note: May be called by translated primitive code. */

	/* StackInterpreter>>#fetchInteger:ofObject: */

sqInt
fetchIntegerofObject(sqInt fieldIndex, sqInt objectPointer)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt intOop;

	intOop = longAt((void *)((objectPointer + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));

	/* begin checkedIntegerValueOf: */
	if ((((intOop) & 7) == 1)) {
		return (intOop >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return 0;
	}
}