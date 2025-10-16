/* Extracted from interp.c:14902 (function primitiveDivLargeIntegers). */

EXPORT(void)
primitiveDivLargeIntegers(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqLong a;
    sqInt aIsNegative;
    usqLong b;
    sqInt bIsNegative;
    sqInt oopArg;
    sqInt oopRcvr;
    sqInt oopResult;
    usqLong rem;
    usqLong result;
    char *sp;

	oopArg = longAt(GIV(stackPointer));
	oopRcvr = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	aIsNegative = isNegativeIntegerValueOf(oopRcvr);
	bIsNegative = isNegativeIntegerValueOf(oopArg);
	a = magnitude64BitValueOf(oopRcvr);
	b = magnitude64BitValueOf(oopArg);
	if (!b) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
	}
	if (GIV(primFailCode)) {
		return;
	}
	result = a / b;
	if (a) {
		if (!(bIsNegative == aIsNegative)) {
			rem = a % b;
			if (rem) {
				result += 1;
			}
		}
	}
	oopResult = magnitude64BitIntegerForneg(result, bIsNegative != aIsNegative);
	if (!GIV(primFailCode)) {
		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),oopResult);
		GIV(stackPointer) = sp;
	}
}