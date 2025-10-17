/* Extracted from interp.c:26173 (function signedMachineIntegerValueOf). */

/*	Answer a signed value of an integer up to the size of a machine word.
	The object may be either a positive SmallInteger or a LargeInteger of size
	<= word size.
 */
/*	inline the common case... */

	/* InterpreterPrimitives>>#signedMachineIntegerValueOf: */

sqIntptr_t
signedMachineIntegerValueOf(sqInt oop)
{
	if ((((oop) & 7) == 1)) {
		return (oop >> 3);
	}
	return signedMachineIntegerValueOfObj(oop);
}