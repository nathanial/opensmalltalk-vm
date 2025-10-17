/* Extracted from interp.c:12911 (function primitiveBitOrLargeIntegers). */

/*	Primitive logical operations for large integers in 64 bit range */

	/* InterpreterPrimitives>>#primitiveBitOrLargeIntegers */

EXPORT(void)
primitiveBitOrLargeIntegers(void)
{
    usqLong integerArg;
    usqLong integerRcvr;
    sqInt oopResult;
    char *sp;

	integerArg = positive64BitValueOf(longAt(stackPointer));
	integerRcvr = positive64BitValueOf(longAt(stackPointer + (1 * BytesPerWord)));
	if (primFailCode) {
		return;
	}
	oopResult = positive64BitIntegerFor(integerRcvr | integerArg);
	if (!primFailCode) {
		/* begin pop:thenPush: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),oopResult);
		stackPointer = sp;
	}
}