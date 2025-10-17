/* Extracted from interp.c:11638 (function primitiveAddLargeIntegers). */

/*	Primitive arithmetic operations for large integers in 64 bit range */

	/* InterpreterPrimitives>>#primitiveAddLargeIntegers */

EXPORT(void)
primitiveAddLargeIntegers(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
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

	oopArg = longAt(GIV(stackPointer));
	oopRcvr = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	aIsNegative = isNegativeIntegerValueOf(oopRcvr);
	bIsNegative = isNegativeIntegerValueOf(oopArg);
	a = magnitude64BitValueOf(oopRcvr);
	b = magnitude64BitValueOf(oopArg);
	if (GIV(primFailCode)) {
		return;
	}
	if (aIsNegative == bIsNegative) {
		if (a > (0xFFFFFFFFFFFFFFFFULL - b)) {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
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
			resultIsNegative = bIsNegative;
		}
	}

	/* Protect against overflow */
	oopResult = magnitude64BitIntegerForneg(result, resultIsNegative);
	if (!GIV(primFailCode)) {
		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),oopResult);
		GIV(stackPointer) = sp;
	}
}