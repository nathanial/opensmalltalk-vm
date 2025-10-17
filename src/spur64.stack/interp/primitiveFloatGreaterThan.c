/* Extracted from interp.c:16084 (function primitiveFloatGreaterThan). */

	/* InterpreterPrimitives>>#primitiveFloatGreaterThan */

static void
primitiveFloatGreaterThan(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    int aBool;
    char *sp;

	aBool = primitiveFloatGreaterthanArg(longAt(GIV(stackPointer) + (1 * BytesPerWord)), longAt(GIV(stackPointer)));
	if (!GIV(primFailCode)) {
		/* begin pop:thenPushBool: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),/* booleanObjectOf: */
			(aBool
				? GIV(trueObj)
				: GIV(falseObj)));
		GIV(stackPointer) = sp;
	}
}