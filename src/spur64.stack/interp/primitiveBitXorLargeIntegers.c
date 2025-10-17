/* Extracted from interp.c:13144 (function primitiveBitXorLargeIntegers). */

/*	Primitive logical operations for large integers in 64 bit range */

	/* InterpreterPrimitives>>#primitiveBitXorLargeIntegers */

EXPORT(void)
primitiveBitXorLargeIntegers(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqLong integerArg;
    usqLong integerRcvr;
    sqInt oopResult;
    char *sp;

	integerArg = positive64BitValueOf(longAt(GIV(stackPointer)));
	integerRcvr = positive64BitValueOf(longAt(GIV(stackPointer) + (1 * BytesPerWord)));
	if (GIV(primFailCode)) {
		return;
	}
	oopResult = positive64BitIntegerFor(integerRcvr ^ integerArg);
	if (!GIV(primFailCode)) {
		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),oopResult);
		GIV(stackPointer) = sp;
	}
}