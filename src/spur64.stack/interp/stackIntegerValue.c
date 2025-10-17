/* Extracted from interp.c:64236 (function stackIntegerValue). */

/*	In the StackInterpreter stacks grow down. */

	/* StackInterpreter>>#stackIntegerValue: */

sqInt
stackIntegerValue(sqInt offset)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerPointer;

	integerPointer = longAt(GIV(stackPointer) + (offset * BytesPerWord));

	/* begin checkedIntegerValueOf: */
	if ((((integerPointer) & 7) == 1)) {
		return (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return 0;
	}
}