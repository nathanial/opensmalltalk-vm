/* Extracted from interp.c:64444 (function stackSignedMachineIntegerValue). */

/*	In the StackInterpreter stacks grow down. */

	/* StackInterpreter>>#stackSignedMachineIntegerValue: */

sqIntptr_t
stackSignedMachineIntegerValue(sqInt offset)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerPointer;

	integerPointer = longAt(stackPointer + (offset * BytesPerWord));

	/* begin signedMachineIntegerValueOf: */
	if ((((integerPointer) & 7) == 1)) {
		return (integerPointer >> 3);
	}
	return signedMachineIntegerValueOfObj(integerPointer);
}