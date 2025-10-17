/* Extracted from interp.c:64415 (function stackPositiveMachineIntegerValue). */

/*	In the StackInterpreter stacks grow down. */

	/* StackInterpreter>>#stackPositiveMachineIntegerValue: */

usqIntptr_t
stackPositiveMachineIntegerValue(sqInt offset)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerPointer;
    sqInt value;

	integerPointer = longAt(GIV(stackPointer) + (offset * BytesPerWord));

	/* begin positiveMachineIntegerValueOf: */
	if ((((integerPointer) & 7) == 1)) {
		value = (integerPointer >> 3);
		if (value < 0) {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
			return null;
		}
		return value;
	}

	/* don't inline the rare case */
	return positiveMachineIntegerValueOfObj(integerPointer);
}