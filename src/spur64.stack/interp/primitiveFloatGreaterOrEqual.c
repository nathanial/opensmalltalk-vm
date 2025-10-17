/* Extracted from interp.c:16066 (function primitiveFloatGreaterOrEqual). */

	/* InterpreterPrimitives>>#primitiveFloatGreaterOrEqual */

static void
primitiveFloatGreaterOrEqual(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    int aBool;
    char *sp;

	aBool = primitiveFloatGreaterOrEqualtoArg(longAt(stackPointer + (1 * BytesPerWord)), longAt(stackPointer));
	if (!primFailCode) {
		/* begin pop:thenPushBool: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),/* booleanObjectOf: */
			(aBool
				? trueObj
				: falseObj));
		stackPointer = sp;
	}
}