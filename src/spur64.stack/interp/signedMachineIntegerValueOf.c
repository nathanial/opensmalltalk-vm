/* Extracted from interp.c:26173 (function signedMachineIntegerValueOf). */

sqIntptr_t
signedMachineIntegerValueOf(sqInt oop)
{
	if ((((oop) & 7) == 1)) {
		return (oop >> 3);
	}
	return signedMachineIntegerValueOfObj(oop);
}