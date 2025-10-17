/* Extracted from interp.c:61225 (function pushInteger). */

	/* StackInterpreter>>#pushInteger: */

sqInt
pushInteger(sqInt integerValue)
{
    char *sp;

	/* begin push: */
	longAtput((sp = stackPointer - BytesPerWord),(((usqInt)integerValue << 3) | 1));
	stackPointer = sp;
	return null;
}