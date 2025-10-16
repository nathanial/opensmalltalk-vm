/* Extracted from interp.c:18749 (function primitiveLessOrEqualLargeIntegers). */

EXPORT(void)
primitiveLessOrEqualLargeIntegers(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqLong integerArg;
    sqLong integerRcvr;
    char *sp;

	integerArg = signed64BitValueOf(longAt(GIV(stackPointer)));
	integerRcvr = signed64BitValueOf(longAt(GIV(stackPointer) + (1 * BytesPerWord)));
	if (!GIV(primFailCode)) {
		/* begin pop:thenPushBool: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),/* booleanObjectOf: */
			(integerRcvr <= integerArg
				? GIV(trueObj)
				: GIV(falseObj)));
		GIV(stackPointer) = sp;
	}
}