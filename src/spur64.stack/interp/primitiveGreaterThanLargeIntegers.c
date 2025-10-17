/* Extracted from interp.c:17532 (function primitiveGreaterThanLargeIntegers). */

/*	Primitive comparison operations for large integers in 64 bit range */

	/* InterpreterPrimitives>>#primitiveGreaterThanLargeIntegers */

EXPORT(void)
primitiveGreaterThanLargeIntegers(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqLong integerArg;
    sqLong integerRcvr;
    char *sp;

	integerArg = signed64BitValueOf(longAt(GIV(stackPointer)));
	integerRcvr = signed64BitValueOf(longAt(GIV(stackPointer) + (1 * BytesPerWord)));
	if (!GIV(primFailCode)) {
		/* begin pop:thenPushBool: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),/* booleanObjectOf: */
			(integerRcvr > integerArg
				? GIV(trueObj)
				: GIV(falseObj)));
		GIV(stackPointer) = sp;
	}
}