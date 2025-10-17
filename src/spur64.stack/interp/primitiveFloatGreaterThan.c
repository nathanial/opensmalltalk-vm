/* Extracted from interp.c:16084 (function primitiveFloatGreaterThan). */

	/* InterpreterPrimitives>>#primitiveFloatGreaterThan */

static void
primitiveFloatGreaterThan(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    int aBool;
    char *sp;

	aBool = primitiveFloatGreaterthanArg(longAt(stackPointer + (1 * BytesPerWord)), longAt(stackPointer));
	if (!primFailCode) {
		/* begin pop:thenPushBool: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),/* booleanObjectOf: */
			(aBool
				? trueObj
				: falseObj));
		stackPointer = sp;
	}
}