/* Extracted from interp.c:64415 (function stackPositiveMachineIntegerValue). */

/*	In the StackInterpreter stacks grow down. */

	/* StackInterpreter>>#stackPositiveMachineIntegerValue: */

usqIntptr_t
stackPositiveMachineIntegerValue(sqInt offset)
{
    sqInt integerPointer;
    sqInt value;

	integerPointer = longAt(stackPointer + (offset * BytesPerWord));

	/* begin positiveMachineIntegerValueOf: */
	if ((((integerPointer) & 7) == 1)) {
		value = (integerPointer >> 3);
		if (value < 0) {
			/* begin primitiveFail */
			if (!primFailCode) {
				primFailCode = 1;
			}
			return null;
		}
		return value;
	}

	/* don't inline the rare case */
	return positiveMachineIntegerValueOfObj(integerPointer);
}