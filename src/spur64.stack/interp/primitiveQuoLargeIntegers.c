/* Extracted from interp.c:21337 (function primitiveQuoLargeIntegers). */

/*	Primitive arithmetic operations for large integers in 64 bit range */

	/* InterpreterPrimitives>>#primitiveQuoLargeIntegers */

EXPORT(void)
primitiveQuoLargeIntegers(void)
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

	oopArg = longAt(stackPointer);
	oopRcvr = longAt(stackPointer + (1 * BytesPerWord));
	aIsNegative = isNegativeIntegerValueOf(oopRcvr);
	bIsNegative = isNegativeIntegerValueOf(oopArg);
	a = magnitude64BitValueOf(oopRcvr);
	b = magnitude64BitValueOf(oopArg);
	if (!b) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
	}
	if (primFailCode) {
		return;
	}
	result = a / b;
	oopResult = magnitude64BitIntegerForneg(result, bIsNegative != aIsNegative);
	if (!primFailCode) {
		/* begin pop:thenPush: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),oopResult);
		stackPointer = sp;
	}
}