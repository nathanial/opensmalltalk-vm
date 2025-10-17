/* Extracted from interp.c:14857 (function primitiveDivideLargeIntegers). */

/*	Primitive arithmetic operations for large integers in 64 bit range */

	/* InterpreterPrimitives>>#primitiveDivideLargeIntegers */

EXPORT(void)
primitiveDivideLargeIntegers(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqLong a;
    sqInt aIsNegative;
    usqLong b;
    sqInt bIsNegative;
    sqInt oopArg;
    sqInt oopRcvr;
    sqInt oopResult;
    usqLong result;
    char *sp;

	oopArg = longAt(GIV(stackPointer));
	oopRcvr = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	aIsNegative = isNegativeIntegerValueOf(oopRcvr);
	bIsNegative = isNegativeIntegerValueOf(oopArg);
	a = magnitude64BitValueOf(oopRcvr);
	b = magnitude64BitValueOf(oopArg);
	if (GIV(primFailCode)) {
		return;
	}

	/* check for exact division */
	if (!((b != 0)
		 && ((a % b) == 0))) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}
	result = a / b;
	oopResult = magnitude64BitIntegerForneg(result, aIsNegative != bIsNegative);
	if (!GIV(primFailCode)) {
		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),oopResult);
		GIV(stackPointer) = sp;
	}
}