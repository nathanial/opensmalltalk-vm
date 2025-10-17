/* Extracted from interp.c:25017 (function primitiveSubtractLargeIntegers). */

/*	Primitive arithmetic operations for large integers in 64 bit range */

	/* InterpreterPrimitives>>#primitiveSubtractLargeIntegers */

EXPORT(void)
primitiveSubtractLargeIntegers(void)
{
    usqLong a;
    sqInt aIsNegative;
    usqLong b;
    sqInt bIsNegative;
    sqInt oopArg;
    sqInt oopRcvr;
    sqInt oopResult;
    usqLong result;
    sqInt resultIsNegative;
    char *sp;

	oopArg = longAt(stackPointer);
	oopRcvr = longAt(stackPointer + (1 * BytesPerWord));
	aIsNegative = isNegativeIntegerValueOf(oopRcvr);
	bIsNegative = isNegativeIntegerValueOf(oopArg);
	a = magnitude64BitValueOf(oopRcvr);
	b = magnitude64BitValueOf(oopArg);
	if (primFailCode) {
		return;
	}
	if (aIsNegative != bIsNegative) {
		if (a > (0xFFFFFFFFFFFFFFFFULL - b)) {
			/* begin primitiveFail */
			if (!primFailCode) {
				primFailCode = 1;
			}
			return;
		}
		result = a + b;
		resultIsNegative = aIsNegative;
	}
	else {
		if (a >= b) {
			result = a - b;
			resultIsNegative = aIsNegative;
		}
		else {
			result = b - a;
			resultIsNegative = !aIsNegative;
		}
	}

	/* Protect against overflow */
	oopResult = magnitude64BitIntegerForneg(result, resultIsNegative);
	if (!primFailCode) {
		/* begin pop:thenPush: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),oopResult);
		stackPointer = sp;
	}
}